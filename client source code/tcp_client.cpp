// I/O header files
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// cpp header files
#include <string>
#include <cstring>
#include <algorithm>

// socket header files
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // fork()
#include <arpa/inet.h>
#include <signal.h>

// openSSL header files
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

// self define header files
#include "appSpecConst.h"
#include "tcp_client_FSM.h"
#include "myUIUX.h"
#include "ssl_for_client.h"



int main() {

    // Initialize SSL Context text
    SSL_CTX* ctx = CUSTOM::InitClientCTX();
    CUSTOM::LoadCertificates(ctx);

    // SSL socket
    SSL* client_ssl = nullptr;

    // create a socket
    // AF_INET for IPv4
    // SOCK_STREAM for TCP
    int client_socket;
    client_socket = socket(MY_NET_FAMILY, SOCK_STREAM, 0);

    // set up server Port and IP
    struct sockaddr_in server_address;
    server_address.sin_family = MY_NET_FAMILY;

    // string to receive server message
    char server_response[MAX_STRING_LENGTH];

    // connection establishment loop
    std::string IP = "0.0.0.0";
    while(true)
    {
        // ask for port to connect
        std::cout << "Please enter the desired IP: ";
        std::cin >> IP;
        std::cin.ignore(MAX_STRING_LENGTH, '\n');

        int PORT = -1;
        std::cout << "Please enter the desired port: ";
        std::cin >> PORT;
        std::cin.ignore(MAX_STRING_LENGTH, '\n');
        while (PORT < 1024 || PORT > 65535)
        {
            std::cout << "The port number should be in range 1024 ~ 65535!\n\n";
            std::cout << "Please enter the desired port: ";
            std::cin >> PORT;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');
        }

        server_address.sin_port = htons(PORT);
        server_address.sin_addr.s_addr = inet_addr(IP.c_str());

        // establish connection
        int connection_status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

        // check for error with the connection
        if (connection_status == -1) {
            std::cout << "Fail to connect to server.\nRetry later.\n\n";
        }
        else {
            // Connect ctx and SSL
            client_ssl = SSL_new(ctx);
            SSL_set_fd(client_ssl, client_socket);
            // Establish SSL connection
            if (SSL_connect(client_ssl) == -1)
            {
                ERR_print_errors_fp(stderr);
                continue;
            }
            else
            {
                printf("Connected with %s encryption\n", SSL_get_cipher(client_ssl));
                CUSTOM::ShowCerts(client_ssl);
            }
            // receive data from the server
            SSL_read(client_ssl, &server_response, sizeof(server_response));
            std::cout << "Server Replied: " << server_response << std::endl;
            memset(server_response, '\0', sizeof(server_response));
            break;
        }
    }

    // main application loop
    CUSTOM::ClientFSM client_fsm;
    client_fsm.refreshInterface(0);
    while(true) 
    {
        // Input area
        int users_choice;
        std::cout << "Select: ";
        std::cin >> users_choice;
        std::cin.ignore(MAX_STRING_LENGTH, '\n');

        // Processing user's request
        std::string sendToServer = client_fsm.processRequest(users_choice);
        if (sendToServer == "ERROR") continue; // Error handling
        else if (sendToServer == "microTransaction") { // microtransaction area
            
            std::string to_user;
            std::cout << "Enter who to transfer: ";
            std::cin >> to_user;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');

            std::string amount;
            std::cout << "Enter amount (<= 20000): ";
            std::cin >> amount;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');
            while (std::stoi(amount) < 0 || std::stoi(amount) > 20000)
            {
                if (std::stoi(amount) < 0)
                {
                    std::cout << "Negative amount, please enter positive amount: ";
                    std::cin >> amount;
                    std::cin.ignore(MAX_STRING_LENGTH, '\n');
                }
                else if (std::stoi(amount) > 20000)
                {
                    std::cout << "Under 20000 can be transferred per transaction, please enter valid amount: ";
                    std::cin >> amount;
                    std::cin.ignore(MAX_STRING_LENGTH, '\n');
                }
            }

            SSL_write(client_ssl, "List", sizeof("List"));

            SSL_read(client_ssl, &server_response, sizeof(server_response));

            // parse server's list
            std::string to_parse(server_response);
            int account_list_begin = 0;
            while (to_parse[account_list_begin++] != '\n') {
                continue;
            }
            while (to_parse[account_list_begin++] != '\n') {
                continue;
            }
            std::string ip_to_send = "";
            std::string port_to_send = "";
            std::string temp = "";
            while ((ip_to_send == "" && port_to_send == "") && account_list_begin < to_parse.length())
            {
                while (to_parse[account_list_begin] != '#') { // parse name
                    temp += to_parse[account_list_begin];
                    account_list_begin++;
                }
                account_list_begin++;
                if (temp == to_user) {
                    while (to_parse[account_list_begin] != '#') { // parse ip
                        ip_to_send += to_parse[account_list_begin];
                        account_list_begin++;
                    }
                    account_list_begin++;
                    while (to_parse[account_list_begin] != '\n') { // parse port
                        port_to_send += to_parse[account_list_begin];
                        account_list_begin++;
                    }
                    account_list_begin++;
                    break;
                }
                else { // pass this line
                    temp = "";
                    while (to_parse[account_list_begin++] != '\n') { 
                        continue;
                    }
                }
            }
            // Payee doesn't exist
            if (temp != to_user)
            {
                std::cout << "Payee doesn't exist!\n";
                continue;
            }
            sendToServer = client_fsm.getActiveState()->microTransaction(to_user, amount, ip_to_send, atoi(port_to_send.c_str()));
            SSL_write(client_ssl, sendToServer.c_str(), sizeof(sendToServer));
            memset(server_response, '\0', sizeof(server_response));
            sleep(1);
            continue;
        }

        SSL_write(client_ssl, sendToServer.c_str(), sizeof(sendToServer));
        SSL_read(client_ssl, &server_response, sizeof(server_response));
        std::cout << "Server Replied: " << server_response << std::endl;
        bool err220 = (server_response[0] == '2' && server_response[1] == '2' && server_response[2] == '0');
        bool err230 = (server_response[0] == '2' && server_response[1] == '3' && server_response[2] == '0');
        if (std::count(sendToServer.begin(), sendToServer.end(), '#') == 1 && !(err220 || err230)) {
            // LOGIN
            std::string temp_user_name = sendToServer.substr(0, sendToServer.find("#"));
            std::string temp_port_string = sendToServer.substr(sendToServer.find("#") + 1, sendToServer.find("\r"));
            //std::cout << temp_port_string << std::endl;
            client_fsm.addState(CUSTOM::StateRef(new CUSTOM::LOGGED_IN(temp_user_name, temp_port_string, atoi(temp_port_string.c_str()))));
            
            pid_t pid;
            pid = fork();
            if (pid < 0)
            {
                // Error
                std::cout << "Error 444: Fork Failed.\n";
                return 0;
            }
            else if (pid == 0)
            {
                // child process
                client_fsm.getActiveState()->startListen(IP, client_ssl);
                kill(pid, SIGKILL);
            }
            else if (pid > 0)
            {
                // parent process
                std::cout << "System Message: Forked.\n";
            }
        }
        else if (sendToServer.substr(0, 4) == "Exit" && server_response[0] == 'B' && server_response[1] == 'y' && server_response[2] == 'e') {
            // LOGOUT
            client_fsm.removeState();
        }
        memset(server_response, '\0', sizeof(server_response));

        client_fsm.refreshInterface(2);
    }

    // close SSL connection
    SSL_shutdown(client_ssl);
    // free SSL
    SSL_free(client_ssl);
    // free ctx
    SSL_CTX_free(ctx);
    // close the socket
    close(client_socket);

    return 0;
}
