/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.  
    Licensed under the Network Next Source Available License 1.0
*/

#include "next_connect_token.h"

bool next_write_connect_token( const next_connect_token_t * token, char * output, const uint8_t * private_key )
{
    next_assert( token );
    next_assert( output );
    next_assert( private_key );

    // todo

    return true;
}

bool next_read_connect_token( next_connect_token_t * token, const char * input, const uint8_t * public_key )
{
    next_assert( token );
    next_assert( input );
    next_assert( public_key );

    // todo
    
    return true;
}
