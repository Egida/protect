/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.
    Licensed under the Network Next Source Available License 2.0
*/

#ifndef SERVER_XDP_CONFIG_H
#define SERVER_XDP_CONFIG_H

#include "server_xdp.h"
#include "server_xdp_shared.h"

struct config_t
{
    uint32_t public_address;
};

bool read_config( struct config_t * config );

#endif // #ifndef SERVER_XDP_CONFIG_H
