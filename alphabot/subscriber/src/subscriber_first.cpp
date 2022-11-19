#include <iostream>
#include <string_view>

#include "general.h"
#include "subscriber.h"
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <mosquitto/Printnum.idl.h>

using namespace std::literals;

constexpr auto Topic = "my/awesome/topic"sv;

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
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        NkKosTransport transport;
        struct mosquitto_Printnum_proxy proxy;
        Handle handle = ServiceLocatorConnect("showapp_connection");
        NkKosTransport_Init(&transport, handle, NK_NULL, 0);
        nk_iid_t riid = ServiceLocatorGetRiid(handle, "mosquitto.Printnum.printnum");

        mosquitto_Printnum_proxy_init(&proxy, &transport.base, riid);

        mosquitto_Printnum_Printnum_req req;
        mosquitto_Printnum_Printnum_res res;
        req.value = atoi(payload.data()); 
        mosquitto_Printnum_Printnum(&proxy.base, &req, NULL, &res, NULL);
    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
