#include <iostream>
#include <string_view>
#include <string>
#include "general.h"
#include "subscriber.h"
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <mosquitto/Printnum.idl.h>

using namespace std::literals;

constexpr auto Topic = "abot/command"sv;

#include <iostream>
#include <fstream>

#include "json.hpp"
#include <list>

#define STOP 0
#define STRAIGHT 1
#define TURN 2
#define SPEED 100

using namespace std;
using json = nlohmann::json;

tuple<int, int, int> cmdAuto(const json &cmd){
    cout << "markers_floor" << endl;
    for (int i = 0;i < cmd["markers_screen"].size();++i){
        json point = cmd["markers_screen"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
    cout << "markers_floor" << endl;
    for (int i = 0;i < cmd["markers_floor"].size();++i){
        json point = cmd["markers_floor"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
    cout << "targets_screen" << endl;
    for (int i = 0;i < cmd["targets_screen"].size();++i){
        json point = cmd["targets_screen"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
    cout << "robot_screen" << endl;
    for (int i = 0;i < cmd["robot_screen"].size();++i){
        json point = cmd["robot_screen"][i];
        cout << point["id"] << " " << point["x"] << " " << point["y"] << endl;
    }
}

tuple<int, int, int> cmdFwd(const json &cmd){
    return make_tuple(STRAIGHT, int(stod(string(cmd["val"]))*1000), SPEED);
}

tuple<int, int, int> cmdBck(const json &cmd){
    return make_tuple(STRAIGHT, int(stod(string(cmd["val"]))*1000), -SPEED);
}

tuple<int, int, int> cmdLft(const json &cmd){
    return make_tuple(TURN, int(stod(string(cmd["val"]))*1000), SPEED);
}

tuple<int, int, int> cmdRht(const json &cmd){
    return make_tuple(TURN, int(stod(string(cmd["val"]))*1000), -SPEED);
}

tuple<int, int, int> cmdStp(const json &cmd){
    return make_tuple(0, 0, 0);
}

tuple<int, int, int> parseJsonCommand(const string &StrCommand){
    json cmd = json::parse(StrCommand);
    if (cmd["cmd"] == "auto")
        return cmdAuto(cmd);
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

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}


void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
	const std::string tmp_string(static_cast<const char*>(message->payload));
	NkKosTransport transport;
        struct mosquitto_Printnum_proxy proxy;
        Handle handle = ServiceLocatorConnect("showapp_connection");
        NkKosTransport_Init(&transport, handle, NK_NULL, 0);
        nk_iid_t riid = ServiceLocatorGetRiid(handle, "mosquitto.Printnum.printnum");

        mosquitto_Printnum_proxy_init(&proxy, &transport.base, riid);

        mosquitto_Printnum_Printnum_req req;
        mosquitto_Printnum_Printnum_res res;

	tuple <int, int, int> answer = parseJsonCommand(tmp_string);
	req.value1 = get<0>(answer);
	req.value2 = get<1>(answer);
	req.value3 = get<2>(answer);

	mosquitto_Printnum_Printnum(&proxy.base, &req, NULL, &res, NULL);
    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
