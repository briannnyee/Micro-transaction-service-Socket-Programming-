#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <queue>
#include <pthread.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "user_map.h"


namespace CUSTOM
{
    class ThreadPool 
    {
    private:
        int pool_size;
        pthread_mutex_t mutex;
        pthread_cond_t condition_var;
        pthread_t *pool;
        std::queue<Client> tasks;
        static void *thread_func_helper(void*);
        void thread_func(void*);
        void handle_connection(Client);
        UserMap users;
        int free_thread;
    public:
        ThreadPool(int);
        void connect(Client&);
    };
}

#endif