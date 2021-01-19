#ifndef _SERVER_H_
#define _SERVER_H_

// I/O header files
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// cpp header files
#include <string>
#include <cstring>
#include <algorithm>
#include <queue>

// socket header files
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // fork()
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h> // thread()

// custom header files
#include "thread_pool.h"
#include "client_and_user.h"
#include "appSpecConst.h"

namespace CUSTOM
{
    class Server
    {
    private:
        ThreadPool thread_pool;
        int server_socket;
        struct sockaddr_in server_address;
        SSL_CTX *ctx;
        std::string cert_file_path = "./server.crt";
        std::string key_file_path = "./server.key";
        SSL_CTX* InitServerCTX();
        void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile);
    public:
        Server(int server_port, int pool_size);
        void Activate();
        int check(int, const char*);
        void ShowCerts(SSL *ssl);
    };
}

#endif