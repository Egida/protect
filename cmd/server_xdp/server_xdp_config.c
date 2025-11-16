/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.
    Licensed under the Network Next Source Available License 2.0
*/

#include "server_xdp_config.h"
#include "server_xdp_shared.h"
#include "platform/platform.h"
#include "shared/shared_base64.h"

#ifdef __linux__
#include <linux/if_ether.h>
#endif // #ifdef __linux__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool read_config( struct config_t * config )
{
    // -----------------------------------------------------------------------------------------------------------------------------

    char * public_address_env = getenv( "SERVER_XDP_PUBLIC_ADDRESS" );
    if ( !public_address_env )
    {
        printf( "\nerror: SERVER_XDP_PUBLIC_ADDRESS not set\n\n" );
        return false;
    }

    if ( !platform_parse_address( public_address_env, &config->public_address, &config->port ) )
    {
        printf( "\nerror: invalid public address '%s'\n\n", public_address_env );
        return false;
    }

    printf( "Public address is %d.%d.%d.%d:%d\n", 
        ((uint8_t*)&config->public_address)[3], 
        ((uint8_t*)&config->public_address)[2], 
        ((uint8_t*)&config->public_address)[1], 
        ((uint8_t*)&config->public_address)[0],
        config->port
    );

    // -----------------------------------------------------------------------------------------------------------------------------

    return true;
}
