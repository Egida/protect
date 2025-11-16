/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.
    Licensed under the Network Next Source Available License 2.0
*/

#ifndef SERVER_XDP_MAIN_H
#define SERVER_XDP_MAIN_H

#include "server_xdp.h"

struct main_t
{
    // void * curl;

    uint64_t start_time;
    uint32_t public_address;
    bool initialized;
    bool shutting_down;
#ifdef __linux__
    int state_fd;
    int config_fd;
#endif // #ifdef __linux__
};

struct config_t;
struct bpf_t;

bool main_init( struct main_t * main, struct config_t * config, struct bpf_t * bpf );

int main_run( struct main_t * main );

void main_shutdown( struct main_t * main );

#endif // #ifndef SERVER_XDP_MAIN_H
