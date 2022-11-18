#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <alphabot/Showapp.edl.h>

static nk_err_t Printnum_impl(struct alphabot_Printnum *self,
                          const struct alphabot_Printnum_Printnum_req *req,
                          const struct nk_arena *req_arena,
                          struct alphabot_Printnum_Printnum_res *res,
                          struct nk_arena *res_arena)
{
    fprintf(stderr,"[ShowApp] Recieved number: %d\n", req->value);
    return NK_EOK;
}

static struct alphabot_Printnum *CreatePrintnumImpl()
{
    static const struct alphabot_Printnum_ops ops = {
        .Printnum = Printnum_impl
    };

    static struct alphabot_Printnum impl = {
        .ops = &ops
    };

    return &impl;
}

int main(void)
{
    NkKosTransport transport;
    ServiceId iid;

    Handle handle = ServiceLocatorRegister("showapp_connection", NULL, 0, &iid);

    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    alphabot_Showapp_entity_req req;
    char req_buffer[alphabot_Showapp_entity_req_arena_size];
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer,
                                        req_buffer + sizeof(req_buffer));

    alphabot_Showapp_entity_res res;
    char res_buffer[alphabot_Showapp_entity_res_arena_size];
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer,
                                        res_buffer + sizeof(res_buffer));

    alphabot_Printnum_component component;

    alphabot_Printnum_component_init(&component, CreatePrintnumImpl());

    alphabot_Showapp_entity entity;
    alphabot_Showapp_entity_init(&entity, &component);

    fprintf(stderr, "Hello I'm showapp application. I'm ready\n");

    /* Dispatch loop implementation. */
    do
    {
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        if (nk_transport_recv(&transport.base,
                              &req.base_,
                              &req_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_recv error\n");
        } else {
            alphabot_Showapp_entity_dispatch(&entity, &req.base_, &req_arena,
                                        &res.base_, &res_arena);
        }
        if (nk_transport_reply(&transport.base,
                               &res.base_,
                               &res_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_reply error\n");
        }
    }
    while (true);

    return EXIT_SUCCESS;
}