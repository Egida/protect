/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.
    Licensed under the Network Next Source Available License 2.0
*/

#include "next.h"

static volatile int quit;

void interrupt_handler( int signal )
{
    (void) signal; quit = 1;
}

int main()
{
    signal( SIGINT, interrupt_handler ); signal( SIGTERM, interrupt_handler );

    if ( !next_init() )
    {
        next_error( "could not initialize network next" );
        return 1;        
    }

    while ( !quit )
    {
        // ...

        next_platform_sleep( 1.0 / 100.0 );
    }

    next_term();

    return 0;
}
