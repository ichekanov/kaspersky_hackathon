#include "subscriber.h"
#include "general.h"
#include "json.hpp"
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

using namespace std::literals;
using namespace std;
using json = nlohmann::json;

constexpr auto Topic = "abot/command"sv;

tuple<int, int, int> cmdAuto(const json &cmd)
{
    cout << "markers_floor" << endl;
    for (int i = 0; i < cmd["markers_screen"].size(); ++i)
    {
        json point = cmd["markers_screen"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
    cout << "markers_floor" << endl;
    for (int i = 0; i < cmd["markers_floor"].size(); ++i)
    {
        json point = cmd["markers_floor"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
    cout << "targets_screen" << endl;
    for (int i = 0; i < cmd["targets_screen"].size(); ++i)
    {
        json point = cmd["targets_screen"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
    cout << "robot_screen" << endl;
    for (int i = 0; i < cmd["robot_screen"].size(); ++i)
    {
        json point = cmd["robot_screen"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
}

tuple<int, int, int> cmdFwd(const json &cmd)
{
    return make_tuple(STRAIGHT, int(stod(string(cmd["val"])) * 1000), SPEED);
}

tuple<int, int, int> cmdBck(const json &cmd)
{
    return make_tuple(STRAIGHT, int(stod(string(cmd["val"])) * 1000), -SPEED);
}

tuple<int, int, int> cmdLft(const json &cmd)
{
    return make_tuple(TURN, int(stod(string(cmd["val"])) * 1000), SPEED);
}

tuple<int, int, int> cmdRht(const json &cmd)
{
    return make_tuple(TURN, int(stod(string(cmd["val"])) * 1000), -SPEED);
}

tuple<int, int, int> cmdStp(const json &cmd)
{
    return make_tuple(STOP, 0, 0);
}

tuple<int, int, int> parseJsonManualCommand(const json &cmd)
{
    if (cmd["cmd"] == "forward")
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
    std::cout << app::AppTag << "Connecting to MQTT Broker with address " << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code " << rc << std::endl;

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
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        std::cout << app::AppTag << "Received message: " << message->payload << std::endl;
        const std::string msg_string(static_cast<const char *>(message->payload));
        json cmd = json::parse(msg_string);

        if (cmd["cmd"] != "auto")
        {
            if (flag_auto_on)
                flag_auto_on = false;
            tuple<int, int, int> answer = parseJsonManualCommand(cmd);
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
            // this->instructions = generateInstructions(beginning_coordinates, getBotCoordinates(cmd));
            auto_started += 60 * 60 * 1000;
        }
    }
}

void Subscriber::on_subscribe(__rtl_unused int mid, __rtl_unused int qos_count, __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}

void Subscriber::run_forever(int timeout = -1, int max_packets = 1)
{
    while (true)
    {
        this->loop(timeout, max_packets);
        if (!this->instructions.empty() && KnGetMSecSinceStart() > this->next_execution)
        {
            auto instruction = this->instructions.front();
            this->instructions.pop_front();
            this->next_execution = KnGetMSecSinceStart() + get<1>(instruction);
            this->execute_instruction(get<0>(instruction), get<1>(instruction), get<2>(instruction));
        }
        if (this->instructions.empty() && flag_auto_on && KnGetMSecSinceStart() > this->next_execution)
        {
            flag_auto_on = false;
            this->execute_instruction(STOP, 0, 0);
        }
    }
}

std::pair<double, double> Subscriber::getBotCoordinates(const json &cmd)
{
    return make_pair(stod(string(cmd["robot_floor"]["x"])), stod(string(cmd["robot_floor"]["y"])));
}
