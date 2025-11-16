/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.
    Licensed under the Network Next Source Available License 2.0
*/

#include "platform/platform.h"
#include "shared/shared_encoding.h"

#include "server_xdp_main.h"
#include "server_xdp_config.h"
#include "server_xdp_shared.h"
#include "server_xdp_bpf.h"

#include <curl/curl.h>
#include <time.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>

#ifdef __linux__
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <arpa/inet.h>
#endif // #ifdef __linux__

bool main_init( struct main_t * main, struct config_t * config, struct bpf_t * bpf )
{
    main->start_time = time( NULL );
    main->port = config->port;
    main->public_address = config->public_address;

    struct server_xdp_config xdp_config;
    memset( &xdp_config, 0, sizeof(xdp_config) );

#ifdef __linux__

    main->config_fd = bpf->config_fd;
    main->state_fd = bpf->state_fd;

    __u32 key = 0;
    int err = bpf_map_update_elem( main->config_fd, &key, &xdp_config, BPF_ANY );
    if ( err != 0 )
    {
        printf( "\nerror: failed to set config: %s\n\n", strerror(errno) );
        return false;
    }

    struct server_xdp_state state;
    state.current_timestamp = time(NULL);
    err = bpf_map_update_elem( main->state_fd, &key, &state, BPF_ANY );
    if ( err != 0 )
    {
        printf( "\nerror: failed to set state: %s\n\n", strerror(errno) );
        return false;
    }

#endif // #ifdef __linux__

    return true;
}

bool main_update( struct main_t * main );

extern bool quit;
extern bool clean_shutdown;

int main_run( struct main_t * main )
{
    fflush( stdout );

    while ( !quit )
    {
        #ifdef __linux__
        int key = 0;
        struct server_xdp_state state;
        state.current_timestamp = time(NULL);
        int err = bpf_map_update_elem( main->state_fd, &key, &state, BPF_ANY );
        if ( err != 0 )
        {
            printf( "\nwarning: failed to set state: %s\n\n", strerror(errno) );
        }
        #endif // #ifdef __linux__

        platform_sleep( 1.0 );
    }

    if ( clean_shutdown )
    {
        printf( "\nClean shutdown...\n" );

        fflush( stdout );

        main->shutting_down = true;

        // ...

        printf( "Clean shutdown completed\n" );

        fflush( stdout );        
    }
    else
    {
        printf( "\nHard shutdown!\n" );

        fflush( stdout );        
    }

    return true;
}

bool main_update( struct main_t * main )
{
    // ...
    return true;
}

void main_shutdown( struct main_t * main )
{
#if 0

    if ( main->curl )
    {
        curl_easy_cleanup( main->curl );
    }

#endif // #if 0

    memset( main, 0, sizeof(struct main_t) );
}

// -----------------------------------------------------------------------------------------------------------------------------
