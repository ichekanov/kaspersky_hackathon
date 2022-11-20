#include "subscriber.h"
#include "general.h"
#include "json.hpp"
#include <cmath>
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <coresrv/time/time_api.h>
#include <fstream>
#include <iostream>
#include <list>
#include <mosquitto/Printnum.idl.h>
#include <string>
#include <string_view>

#define STOP 0
#define STRAIGHT 1
#define TURN 2
#define SPEED 100
#define CALIBRATION_DURATION 3000
#define PI 3.14159265
#define ROTGRADPERSEC 180.0

using namespace std::literals;
using namespace std;
using json = nlohmann::json;

constexpr auto Topic = "abot/command2"sv;

double calibration()
{
    // TODO: REMOVE!
    return 0;
}

double vectorAngleAbsolute(double x1, double y1, double x2, double y2)
{
    return atan2(y2 - y1, x2 - x1) * 180 / PI;
}

double angleToRotate(double startAngle, double x1, double y1, double x2, double y2)
{
    double directionAngle = vectorAngleAbsolute(x1, y1, x2, y2);
    return directionAngle - startAngle;
}

list<tuple<int, int, int>> getCommands(json cmd)
{
    double x1 = stod(string(cmd["robot_floor"][0]["x"])), y1 = stod(string(cmd["robot_floor"][0]["y"]));
    double startAngle = calibration();

    list<tuple<int, int, int>> res;
    for (int i = 0; i < cmd["targets_floor"].size(); ++i)
    {
        double x2 = stod(string(cmd["targets_floor"][i]["x"])), y2 = stod(string(cmd["targets_floor"][i]["y"]));
        double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        double rotAngle = angleToRotate(startAngle, x1, y1, x2, y2);
        int rotTime = int((abs(rotAngle) / ROTGRADPERSEC) * 1000); // в милисекундах

        // TODO: Не перепутаны ли знаки
        res.push_back(make_tuple(TURN, rotTime, (rotAngle > 0 ? ROTGRADPERSEC : -ROTGRADPERSEC)));
        res.push_back(make_tuple(STRAIGHT, distance * 100000 / (SPEED), SPEED));

        startAngle = vectorAngleAbsolute(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
    return res;
}

list<tuple<int, int, int>> cmdFwd(const json &cmd)
{
    list<tuple<int, int, int>> a;
    a.push_back(make_tuple(STRAIGHT, int(stod(string(cmd["val"])) * 1000), SPEED));
    return a;
}

list<tuple<int, int, int>> cmdBck(const json &cmd)
{
    list<tuple<int, int, int>> a;
    a.push_back(make_tuple(STRAIGHT, int(stod(string(cmd["val"])) * 1000), -SPEED));
    return a;
}

list<tuple<int, int, int>> cmdLft(const json &cmd)
{
    list<tuple<int, int, int>> a;
    a.push_back(make_tuple(TURN, int(stod(string(cmd["val"])) * 1000), SPEED));
    return a;
}

list<tuple<int, int, int>> cmdRht(const json &cmd)
{
    list<tuple<int, int, int>> a;
    a.push_back(make_tuple(TURN, int(stod(string(cmd["val"])) * 1000), -SPEED));
    return a;
}

list<tuple<int, int, int>> cmdStp(const json &cmd)
{
    list<tuple<int, int, int>> a;
    a.push_back(make_tuple(STOP, 0, 0));
    return a;
}

list<tuple<int, int, int>> parseJsonManualCommand(const json &cmd)
{
    if (cmd["cmd"] == "auto")
        return getCommands(cmd);
    else if (cmd["cmd"] == "forward")
        return cmdFwd(cmd);
    else if (cmd["cmd"] == "back")
        return cmdBck(cmd);
    else if (cmd["cmd"] == "left")
        return cmdLft(cmd);
    else if (cmd["cmd"] == "right")
        return cmdRht(cmd);
    else if (cmd["cmd"] == "stop")
        return cmdStp(cmd);
}

//----------------------------------------------------------------------------------------------------

Subscriber::Subscriber(const char *id, const char *host, int port) : mosquittopp(id)
{
    // std::cout << app::AppTag << "Connecting to MQTT Broker with address " << host << " and port " << port <<
    // std::endl;
    const int keepAlive = 60;
    connect(host, port, keepAlive);
    fprintf(stderr, "Connecting to MQTT Broker with address %s and port %d\n", host, port);
    this->is_connected_mqtt = false;
}

void Subscriber::on_connect(int rc)
{
    // std::cout << app::AppTag << "Subscriber connected with code " << rc << std::endl;
    fprintf(stderr, "Subscriber connected with code %d\n", rc);
    fprintf(stderr, "Subscribing to topic %s\n", Topic.data());
    this->is_connected_mqtt = true;
    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}

void Subscriber::execute_instruction(int cmd, int duration, int speed)
{
    NkKosTransport transport;
    struct mosquitto_Printnum_proxy proxy;
    Handle handle = ServiceLocatorConnect("showapp_connection");
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);
    nk_iid_t riid = ServiceLocatorGetRiid(handle, "mosquitto.Printnum.printnum");
    mosquitto_Printnum_proxy_init(&proxy, &transport.base, riid);
    mosquitto_Printnum_Printnum_req req;
    mosquitto_Printnum_Printnum_res res;
    req.value1 = cmd;
    req.value2 = duration;
    req.value3 = speed;
    mosquitto_Printnum_Printnum(&proxy.base, &req, NULL, &res, NULL);
    // std::cerr << "message sent" << std::endl;
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string msg_string(static_cast<const char *>(message->payload));
        // std::cout << app::AppTag << "Received message: " << msg_string << std::endl;
        fprintf(stderr, "Received message: %s\n", msg_string.c_str());
        json cmd = json::parse(msg_string);

        if (cmd["cmd"] != "auto")
        {
            if (flag_auto_on)
                flag_auto_on = false;
            list<tuple<int, int, int>> instruction = parseJsonManualCommand(cmd);
            tuple<int, int, int> answer = instruction.front();
            fprintf(stderr, "Sending command: %d %d %d\n", get<0>(answer), get<1>(answer), get<2>(answer));
            this->execute_instruction(get<0>(answer), get<1>(answer), get<2>(answer));
        }
        if (cmd["cmd"] == "auto" && !flag_auto_on)
        {
            beginning_coordinates = getBotCoordinates(cmd);
            this->execute_instruction(STRAIGHT, CALIBRATION_DURATION, SPEED);
            flag_auto_on = true;
            auto_started = KnGetMSecSinceStart();
        }
        if (flag_auto_on && KnGetMSecSinceStart() - auto_started > CALIBRATION_DURATION)
        {
            this->instructions = parseJsonManualCommand(cmd);
            auto_started += 60 * 60 * 1000;
        }
    }
}

void Subscriber::on_subscribe(__rtl_unused int mid, __rtl_unused int qos_count, __rtl_unused const int *granted_qos)
{
    // std::cout << app::AppTag << "Subscription succeeded." << std::endl;
    fprintf(stderr, "Subscription succeeded.\n");
}

void Subscriber::run_forever(int timeout, int max_packets)
{
    while (!this->is_connected_mqtt()) {}   
    this->loop_start();
    while (true)
    {
        if (!this->instructions.empty() && KnGetMSecSinceStart() > this->next_execution)
        {
            auto instruction = this->instructions.front();
            this->instructions.pop_front();
            this->next_execution = KnGetMSecSinceStart() + get<1>(instruction);
            this->execute_instruction(get<0>(instruction), get<1>(instruction), get<2>(instruction));
        }
        else if (this->instructions.empty() && flag_auto_on && KnGetMSecSinceStart() > this->next_execution)
        {
            this->flag_auto_on = false;
            this->execute_instruction(STOP, 0, 0);
        }
        else if (KnGetMSecSinceStart() % 10 == 0)
            this->execute_instruction(-1, 0, 0);

        // if (KnGetMSecSinceStart()%100 == 0)
            // this->loop();
    }
}

std::pair<double, double> Subscriber::getBotCoordinates(const json &cmd)
{
    return make_pair(stod(string(cmd["robot_floor"]["x"])), stod(string(cmd["robot_floor"]["y"])));
}
