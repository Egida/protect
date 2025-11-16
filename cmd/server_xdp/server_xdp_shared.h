/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.
    Licensed under the Network Next Source Available License 2.0
*/

#pragma once

#include "server_xdp_constants.h"

#ifdef __linux__
#include <linux/types.h>
#else // #ifdef __linux__
#define __u64 uint64_t
#define __u32 uint32_t
#define __u16 uint16_t
#define __u8 uint8_t
#endif // #ifdef __linux__

#define SIGN_PUBLIC_KEY_BYTES               32
#define SIGN_PRIVATE_KEY_BYTES              64

#define SECRETBOX_PRIVATE_KEY_BYTES         32

struct server_xdp_config
{
    __u32 public_address;                                               // big endian
};

struct server_xdp_state
{
    __u64 current_timestamp;
};
