#ifndef _APP_SPEC_CONST_H_
#define _APP_SPEC_CONST_H_

#include <netinet/in.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// maximum receive message length
#define MAX_STRING_LENGTH 1024

// choose AF_NET for IPv4 
#define MY_NET_FAMILY AF_INET

#define SOCKETERROR -1
#define THREAD_POOL_SIZE 2
#define SERVER_BACKLOG 10
#define IP "0.0.0.0"
#define SERVER_PORT 8080

#endif