#ifndef _APP_SPEC_CONST_H_
#define _APP_SPEC_CONST_H_

#include <netinet/in.h>

// maximum receive message length
#define MAX_STRING_LENGTH 1024

// choose AF_NET for IPv4 
#define MY_NET_FAMILY AF_INET

// secure connection files
#define CERT_FILE_PATH "./client.crt"
#define KEY_FILE_PATH "./client.key"

#endif