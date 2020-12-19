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

// self define header files
#include "appSpecConst.h"
#include "tcp_client_FSM.h"
#include "myUIUX.h"


int main() {

    // create a socket
    // AF_INET for IPv4
    // SOCK_STREAM for TCP
    int client_socket;
    client_socket = socket(MY_NET_FAMILY, SOCK_STREAM, 0);
    
    // establish child process
    // pid_t pid;

    // pid = fork();

    // if (pid < 0) {
    //     // Error
    //     std::cout << "Error 444: Fork Failed.\n";
    //     return 0;
    // }
    // else if (pid == 0) {
    //     // Child Process

    // }
    // else if (pid > 0) {
    //     // Parent Process

    // }

    // set up server Port and IP
    struct sockaddr_in server_address;
    server_address.sin_family = MY_NET_FAMILY;

    // string to receive server message
    char server_response[MAX_STRING_LENGTH];

    // connection establishment loop
    while(true)
    {
        // ask for port to connect
        std::string IP = "0.0.0.0";
        std::cout << "Please enter the desired IP: ";
        std::cin >> IP;
        std::cin.ignore(MAX_STRING_LENGTH, '\n');

        int PORT = 100;
        std::cout << "Please enter the desired port: ";
        std::cin >> PORT;
        std::cin.ignore(MAX_STRING_LENGTH, '\n');

        server_address.sin_port = htons(PORT);
        server_address.sin_addr.s_addr = inet_addr(IP.c_str());

        // establish connection
        int connection_status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

        // check for error with the connection
        if (connection_status == -1) {
            std::cout << "Fail to connect to server.\nRetry later.\n\n";
        }
        else {
            // receive data from the server
            recv(client_socket, &server_response, sizeof(server_response), 0);
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
        std::cout << "6\n";
        if (sendToServer == "ERROR") continue; // Error handling
        send(client_socket, sendToServer.c_str(), sizeof(sendToServer), 0);
        std::cout << "7\n";

        recv(client_socket, &server_response, sizeof(server_response), 0);
        std::cout << "Server Replied: " << server_response << std::endl;
        if (std::count(sendToServer.begin(), sendToServer.end(), '#') == 1 && !(server_response[0] == '2' && server_response[1] == '2' && server_response[2] == '0')) {
            // LOGIN
            client_fsm.addState(CUSTOM::StateRef(new CUSTOM::LOGGED_IN(sendToServer.substr(0, sendToServer.find("#")))));
        }
        else if (sendToServer.substr(0, 4) == "Exit" && server_response[0] == 'B' && server_response[1] == 'y' && server_response[2] == 'e') {
            // LOGOUT
            client_fsm.removeState();
        }
        memset(server_response, '\0', sizeof(server_response));

        client_fsm.refreshInterface(2);
    }

    // close the socket
    close(client_socket);

    return 0;
}
