#include "Motor.hpp"
#include <bsp/bsp.h>
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <coresrv/time/time_api.h>
#include <gpio/gpio.h>
#include <iostream>
#include <mosquitto/Showapp.edl.h>
#include <rtl/countof.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>

#define GPIO_PIN_NUM RTL_UINT32_C(28)
#define DELAY_S 2
#define HW_MODULE_NAME "gpio0"
#define HW_MODULE_CFG "raspberry_pi4b.default"
#define GPIO_HIGH_LEVEL 1
#define GPIO_LOW_LEVEL 0

#define GPIO_PIN_UART_TX 14
#define GPIO_PIN_UART_RX 15

#define GPIO_PIN_MOTOR_IN1 12
#define GPIO_PIN_MOTOR_IN2 13
#define GPIO_PIN_MOTOR_ENA 6
#define GPIO_PIN_MOTOR_IN3 20
#define GPIO_PIN_MOTOR_IN4 21
#define GPIO_PIN_MOTOR_ENB 26

Motor motor(GPIO_PIN_MOTOR_IN1, GPIO_PIN_MOTOR_IN2, GPIO_PIN_MOTOR_ENA, GPIO_PIN_MOTOR_IN3, GPIO_PIN_MOTOR_IN4, GPIO_PIN_MOTOR_ENB);


static nk_err_t Printnum_impl(struct mosquitto_Printnum *self, const struct mosquitto_Printnum_Printnum_req *req,
                              const struct nk_arena *req_arena, struct mosquitto_Printnum_Printnum_res *res,
                              struct nk_arena *res_arena)
{
    fprintf(stderr, "[ShowApp] Recieved number: %d, %d, %d\n", req->value1, req->value2, req->value3);
    motor.do_instruction(req->value1, req->value2);
    return NK_EOK;
}

static struct mosquitto_Printnum *CreatePrintnumImpl()
{
    static const struct mosquitto_Printnum_ops ops = {.Printnum = Printnum_impl};

    static struct mosquitto_Printnum impl = {.ops = &ops};

    return &impl;
}

int main(void)
{
    NkKosTransport transport;
    ServiceId iid;
    Handle handle = ServiceLocatorRegister("showapp_connection", NULL, 0, &iid);
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);
    mosquitto_Showapp_entity_req req;
    char req_buffer[mosquitto_Showapp_entity_req_arena_size];
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer, req_buffer + sizeof(req_buffer));
    mosquitto_Showapp_entity_res res;
    char res_buffer[mosquitto_Showapp_entity_res_arena_size];
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer, res_buffer + sizeof(res_buffer));
    mosquitto_Printnum_component component;
    mosquitto_Printnum_component_init(&component, CreatePrintnumImpl());
    mosquitto_Showapp_entity entity;
    mosquitto_Showapp_entity_init(&entity, &component);

    motor.begin();

    fprintf(stderr, "Hello I'm showapp application. I'm ready\n");

    /* Dispatch loop implementation. */
    while (true)
    {
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        if (nk_transport_recv(&transport.base, &req.base_, &req_arena) != NK_EOK)
            fprintf(stderr, "nk_transport_recv error\n");
        else
            mosquitto_Showapp_entity_dispatch(&entity, &req.base_, &req_arena, &res.base_, &res_arena);
        if (nk_transport_reply(&transport.base, &res.base_, &res_arena) != NK_EOK)
            fprintf(stderr, "nk_transport_reply error\n");
        motor.run();
    }

    return EXIT_SUCCESS;
}
