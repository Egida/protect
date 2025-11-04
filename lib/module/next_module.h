/*
    Network Next Linux kernel module

    This module supports Ubuntu 24.04 LTS
*/

#pragma once

#define NEXT_SIGN_PUBLIC_KEY_BYTES 32

__bpf_kfunc int bpf_next_sha256( void * data, int data__sz, void * output, int output__sz );

struct next_sign_verify_args
{
    __u8 public_key[NEXT_SIGN_PUBLIC_KEY_BYTES];
};

__bpf_kfunc int bpf_next_sign_verify( void * data, int data__sz, void * signature, int signature__sz, struct next_sign_verify_args * args );
