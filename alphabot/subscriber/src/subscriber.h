/* © 2021 AO Kaspersky Lab */
#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H

#include "json.hpp"
#include <coresrv/time/time_api.h>
#include <list>
#include <mosquittopp.h>
#include <rtl/compiler.h>
#include <tuple>

using json = nlohmann::json;

class Subscriber : public mosqpp::mosquittopp
{
  public:
    Subscriber(const char *id, const char *host, int port);
    ~Subscriber(){};

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int mid, __rtl_unused int qos_count, __rtl_unused const int *granted_qos) override;
    void run_forever(int timeout, int max_packets);
    std::pair<double, double> getBotCoordinates(const json &cmd);

  private:
    bool flag_auto_on;
    rtl_size_t auto_started;
    rtl_size_t next_execution;
    std::pair<double, double> beginning_coordinates;
    std::list<std::tuple<int, int, int>> instructions;
    bool is_connected_mqtt;

    void execute_instruction(int value1, int value2, int value3);
};

#endif // _MOSQUITTO_SUBSCRIBER_H
