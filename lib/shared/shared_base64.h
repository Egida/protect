/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.  

    Licensed under the GNU General Public License 3.0 when linked against code that does link to GPL code.

    Licensed under the Network Next Source Available License 2.0 when used in a program that *does not* link to any GPL code.
*/

#ifndef SHARED_BASE64_H
#define SHARED_BASE64_H

#include <stdint.h>
#include <stddef.h>

int shared_base64_decode_data( const char * input, uint8_t * output, size_t output_size );

int shared_base64_decode_string( const char * input, char * output, size_t output_size );

#endif // #ifndef SHARED_BASE64_H
