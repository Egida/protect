/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.  
    Licensed under the Network Next Source Available License 2.0
*/

#pragma once

#ifndef NEXT_CLIENT_SOCKET_H
#define NEXT_CLIENT_SOCKET_H

#include "next_address.h"
#include "next_packets.h"

struct next_client_socket_t;

#define NEXT_CLIENT_SOCKET_CONNECTION_TIMED_OUT    -2
#define NEXT_CLIENT_SOCKET_INIT_TIMED_OUT          -1
#define NEXT_CLIENT_SOCKET_DISCONNECTED             0
#define NEXT_CLIENT_SOCKET_INITIALIZING             1
#define NEXT_CLIENT_SOCKET_CONNECTING               2
#define NEXT_CLIENT_SOCKET_CONNECTED                3

next_client_socket_t * next_client_socket_create( void * context, const char * connect_address );

void next_client_socket_destroy( next_client_socket_t * client_socket );

void next_client_socket_update( next_client_socket_t * client_socket );

void next_client_socket_send_packet( next_client_socket_t * client_socket, const uint8_t * packet_data, int packet_bytes );

int next_client_socket_receive_packet( next_client_socket_t * client_socket, uint8_t * packet_data );

void next_client_socket_disconnect( next_client_socket_t * client_socket );

int next_client_socket_state( next_client_socket_t * client_socket );

uint64_t next_client_socket_session_id( next_client_socket_t * client_socket );

uint64_t next_client_socket_server_id( next_client_socket_t * client_socket );

#endif // #ifndef NEXT_CLIENT_SOCKET_H
