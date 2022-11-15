#pragma GCC diagnostic push
#include <nk/transport.h>
#include <nk/types.h>
#ifndef __hello__Hello__COMPONENT_ENDPOINTS__
#define __hello__Hello__COMPONENT_ENDPOINTS__
enum {
    hello_Hello_iidMax,
};
enum {
    hello_Hello_iidOffset = 0,
};
enum {
    hello_Hello_securityIidMax,
};
enum {
    hello_Hello_component_req_arena_size = 0,
    hello_Hello_component_res_arena_size = 0,
    hello_Hello_component_arena_size = 0,
    hello_Hello_component_req_handles = 0,
    hello_Hello_component_res_handles = 0,
    hello_Hello_component_err_handles = 0,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Hello_iidMax hello_Hello_iidMax
#define Hello_iidOffset hello_Hello_iidOffset
#define Hello_securityIidMax hello_Hello_securityIidMax
#define Hello_component_req_arena_size hello_Hello_component_req_arena_size
#define Hello_component_res_arena_size hello_Hello_component_res_arena_size
#define Hello_component_arena_size hello_Hello_component_arena_size
#define Hello_component_req_handles hello_Hello_component_req_handles
#define Hello_component_res_handles hello_Hello_component_res_handles
#define Hello_component_err_handles hello_Hello_component_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __hello__Hello__COMPONENT_ENDPOINTS__ */

#ifndef __hello__Hello__TASK_ENDPOINTS__
#define __hello__Hello__TASK_ENDPOINTS__
enum {
    hello_Hello_entity_req_arena_size =
    hello_Hello_component_req_arena_size,
    hello_Hello_entity_res_arena_size =
    hello_Hello_component_res_arena_size,
    hello_Hello_entity_arena_size =
    hello_Hello_component_arena_size,
    hello_Hello_entity_req_handles =
    hello_Hello_component_req_handles,
    hello_Hello_entity_res_handles =
    hello_Hello_component_res_handles,
    hello_Hello_entity_err_handles =
    hello_Hello_component_err_handles,
};
#ifdef NK_USE_UNQUALIFIED_NAMES
#define Hello_entity_req_arena_size hello_Hello_entity_req_arena_size
#define Hello_entity_res_arena_size hello_Hello_entity_res_arena_size
#define Hello_entity_arena_size hello_Hello_entity_arena_size
#define Hello_entity_req_handles hello_Hello_entity_req_handles
#define Hello_entity_res_handles hello_Hello_entity_res_handles
#define Hello_entity_err_handles hello_Hello_entity_err_handles
#endif /* NK_USE_UNQUALIFIED_NAMES */
#endif /* __hello__Hello__TASK_ENDPOINTS__ */

#pragma GCC diagnostic pop

