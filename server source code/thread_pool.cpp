#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include "thread_pool.h"

#define MAX_STRING_LENGTH 200

namespace CUSTOM
{
    // https://stackoverflow.com/questions/1151582/pthread-function-from-a-class
    void *ThreadPool::thread_func_helper(void* thread)
    {
        // A static function for casting purpose
        ThreadPool* _this = (ThreadPool*)thread;
        _this->thread_func(thread);
        return nullptr;
    }

    ThreadPool::ThreadPool(int pool_size)
    {
        std::cout << "Initializing thread pool...\n";
        this->pool_size = pool_size;
        this->free_thread = pool_size;
        std::cout << pool_size << " " << free_thread << std::endl;

        // Mutual exclusion
        mutex = PTHREAD_MUTEX_INITIALIZER;

        // Condition variable
        condition_var = PTHREAD_COND_INITIALIZER;

        // Create thread pool
        pool = new pthread_t[pool_size];
        for (int i = 0; i < pool_size; i++) {
            pthread_create(&(this->pool[i]), NULL, &this->thread_func_helper, this);
        }
        std::cout << "Thread pool ready to serve!\n";
    }

    void ThreadPool::connect(Client &newClient)
    {
        pthread_mutex_lock(&(this->mutex));
        tasks.push(newClient);
        pthread_mutex_unlock(&(this->mutex));
        // Notify all that a client has arrived
        // pthread_cond_signal(&(this->condition_var));

    }

    void ThreadPool::thread_func(void* thread)
    {
        // Waiting for clients
        ThreadPool* _this = (ThreadPool*)thread;
        while(true) {
            Client client;

            // 1 thread accesses the tasks queue at the time, others should wait
            pthread_mutex_lock(&(this->mutex));
            // Wait for notification
            // pthread_cond_wait(&(this->condition_var), &(this->mutex));
            if (!tasks.empty()) {
                client = tasks.front();
                tasks.pop();
            }
            pthread_mutex_unlock(&(this->mutex));
            // Got a client, now serve the client
            _this->handle_connection(client);
        }
    }

    void ThreadPool::handle_connection(Client client)
    {
        // Get the client's socket file descriptor
        SSL* client_ssl = client.getSSLSocket();
        // std::cout << "Thread Message: I'm handling!\n";

        if (client_ssl == nullptr) {
            return;
        }

        // Say hello to the client
        std::string server_msg = "";
        server_msg += "HELLOOOOUUUOOO!\n";
        SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
        std::cout << "===Server Message: " << server_msg;
        server_msg = "";

        // Buffer for client input
        char client_input[MAX_STRING_LENGTH] = "";
        memset(client_input, '\0', sizeof(client_input));

        User* user = nullptr;
        
        // main application loop
        while(true)
        {
            SSL_read(client_ssl, client_input, sizeof(client_input));
            std::string input_string(client_input);
            std::cout << "=>Client Message: " << client_input << std::endl;
            memset(client_input, '\0', sizeof(client_input));

            
            // Login
            std::size_t occurences;
            occurences = std::count(input_string.begin(), input_string.end(), '#');
            if (occurences == 1)
            {
                pthread_mutex_lock(&(this->mutex));
                user = users.loginUser(input_string, client);
                pthread_mutex_unlock(&(this->mutex));

                // If not registered
                if (user == nullptr)
                {
                    server_msg += "220 AUTH_FAIL\n";
                    SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                    std::cout << "===>Server Message: " << server_msg;
                    server_msg = "";
                    continue;
                }

                // If someone else's online
                if (user->getIsOnline())
                {
                    server_msg += "230 HAVE_LOGGED_SRY\n";
                    SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                    std::cout << "===>Server Message: " << server_msg;
                    server_msg = "";
                }

                // Login Success
                else
                {
                    // set user status
                    user->setOnline();

                    // Return list message
                    server_msg += users.listUser(std::to_string(user->getBalance()));
                    SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                    std::cout << "===>Server Message: \n" << server_msg;
                    server_msg = "";
                }
                continue;
            }
            
            // Register
            std::size_t foundAt;
            foundAt = input_string.find("REGISTER");
            if (foundAt != std::string::npos)
            {
                pthread_mutex_lock(&(this->mutex));
                bool tempFlag = users.registerUser(input_string.substr(foundAt + 9));
                pthread_mutex_unlock(&(this->mutex));
                if (tempFlag)
                {
                    server_msg += "100 OK\n";
                    SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                    std::cout << "===>Server Message: " << server_msg;
                    server_msg = "";
                }
                else
                {
                    server_msg += "210 NAME_EXIST\n";
                    SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                    std::cout << "===>Server Message: " << server_msg;
                    server_msg = "";
                }
                continue;
            }

            // List
            foundAt = input_string.find("List");
            if (foundAt != std::string::npos)
            {
                pthread_mutex_lock(&(this->mutex));
                server_msg += users.listUser(std::to_string(user->getBalance()));
                pthread_mutex_unlock(&(this->mutex));
                SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                std::cout << "===>Server Message: \n" << server_msg;
                server_msg = "";
                continue;
            }

            // Logout
            foundAt = input_string.find("Exit");
            if (foundAt != std::string::npos)
            {
                pthread_mutex_lock(&(this->mutex));
                users.logoutUser(user);
                pthread_mutex_unlock(&(this->mutex));
                user = nullptr;
                server_msg += "Bye\n";
                SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                std::cout << "===>Server Message: " << server_msg;
                server_msg = "";
                continue;
            }

            // Microtransaction
            foundAt = input_string.find("&");
            if (foundAt != std::string::npos)
            {
                // Parse Info
                std::string payerName = "";
                std::string amount = "";
                std::string payeeName = "";
                std::string temp = "";
                int sharpCnt = 0;
                for (int i = 0; i < input_string.length(); i++)
                {
                    if (input_string[i] == '#' || input_string[i] == '&')
                    {
                        sharpCnt++;
                        if (sharpCnt == 1) // payerName
                        {
                            payerName = temp;
                        } 
                        else if (sharpCnt == 2) // amount
                        {
                            amount = temp;
                        }
                        else // payeeName
                        {
                            payeeName = temp;
                        }
                        temp = "";
                    }
                    else
                    {
                        temp += input_string[i];
                    }
                }

                // Process transaction
                bool res = false;
                pthread_mutex_lock(&(this->mutex));
                res = users.transaction(payerName, std::stoi(amount), payeeName);
                pthread_mutex_unlock(&(this->mutex));
                if (res)
                {
                    server_msg += "Transaction Success! \nCurrent Balance: " + std::to_string(user->getBalance()) + "\n";
                }
                else
                {
                    server_msg += "Transaction Failed!\n";
                }
                // SSL_write(client_ssl, server_msg.c_str(), server_msg.length());
                std::cout << "===>Server Message: " << server_msg;
                server_msg = "";
                continue;
            }

            // only when receiving '' (client close socket) will terminate the service
            // and free the little thread.
            if (user != nullptr)
            {
                // auto logout if connection terminates unexpectedly
                if (user->getIsOnline())
                {
                    users.logoutUser(user);
                }
            }
            break;
        }

        // close SSL connection
        SSL_shutdown(client_ssl);
        // free SSL
        SSL_free(client_ssl);
        // close the socket
        close(client.getSocket());
        std::cout << "Thread Message: Little thread did very well. Returning to thread pool." << std::endl;
    }
}