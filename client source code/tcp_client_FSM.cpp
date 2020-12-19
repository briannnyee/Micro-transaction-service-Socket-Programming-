#include "tcp_client_FSM.h"

namespace CUSTOM
{
    ClientFSM::ClientFSM()
    {
        std::cout << "0\n";
        addState(StateRef(new WELCOME()));
    }

    std::string ClientFSM::processRequest(int featureIndex)
    {
        std::string what_feature = getActiveState()->queryFeature(featureIndex);
        std::string sendToServer = "";

        if (what_feature == "Error 1487")
        {
            std::cout << "Error 1487: Input not valid.\n";
            refreshInterface(2);
            return "ERROR";
        }

        printSeperator();
        std::cout << what_feature << std::endl;
        printSeperator();

        //std::cout << "9\n";
        // Process features and generate message to send (to server)
        if (what_feature == "Login")
        {
            std::string user_name;
            std::cout << "User Name: ";
            std::cin >> user_name;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');

            std::string port_num;
            std::cout << "Port Number: ";
            std::cin >> port_num;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');

            printSeperator();

            sendToServer = user_name + "#" + port_num;// + CRLF;
        }
        else if (what_feature == "No account? Register now")
        {
            std::string user_name;
            std::cout << "Enter user name: ";
            std::cin >> user_name;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');

            std::string deposit_amount;
            std::cout << "Enter initial amount: ";
            std::cin >> deposit_amount;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');

            printSeperator();

            sendToServer = "REGISTER#" + user_name + "#" + deposit_amount; // + CRLF;
        }
        else if (what_feature == "Check Balances")
        {
            sendToServer = "List"; // + CRLF;
        }
        else if (what_feature == "Logout")
        {
            sendToServer = "Exit"; // + CRLF
        }
        else if (what_feature == "Microtransaction")
        {
            sendToServer = "microTransaction";
        }
        //std::cout << "10\n";
        return sendToServer;
    }

    void ClientFSM::refreshInterface(int duration)
    {
        waitAndRefresh(duration);
        getActiveState()->printFeature();

    }

    WELCOME::WELCOME()
    {
        activated_msg = "Welcome, please log in or register.\n";
        state_feature.push_back("Login");
        state_feature.push_back("No account? Register now");
    }

    LOGGED_IN::LOGGED_IN(std::string user_name, std::string port_str, int port_num)
    {
        this->user_name = user_name;
        this->port_str = port_str;
        this->port_num = port_num;
        activated_msg = "Hello" + user_name + ",how may I serve you today?\n";
        state_feature.push_back("Check Balances");
        state_feature.push_back("Microtransaction");
        state_feature.push_back("Logout");
    }

    void LOGGED_IN::microTransaction(std::string to_user, std::string amount, std::string ip_to_send, int port_to_send)
    {
        // create a one-time-use peer socket
        int to_peer_socket;
        to_peer_socket = socket(MY_NET_FAMILY, SOCK_STREAM, 0);

        struct sockaddr_in to_peer_address;
        to_peer_address.sin_family = MY_NET_FAMILY;
        to_peer_address.sin_port = htons(port_to_send);
        to_peer_address.sin_addr.s_addr = inet_addr(ip_to_send.c_str());

        // establish connection
        int connection_status = connect(to_peer_socket, (struct sockaddr*) &to_peer_address, sizeof(to_peer_address));
        char peer_response[MAX_STRING_LENGTH];

        // check for error with the connection
        if (connection_status == -1) {
            std::cout << to_user <<"'s busy.\nRetry later.\n\n";
        }
        else {
            // receive data from the peer
            recv(to_peer_socket, &peer_response, sizeof(peer_response), 0);
            std::cout << user_name << " received: " << peer_response << std::endl;
            memset(peer_response, '\0', sizeof(peer_response));
            std::string temp = "Why not?\n";
            std::string sendToPeer = user_name + "#" + amount + "#" + to_user + "\r\n" + temp;
            send(to_peer_socket, sendToPeer.c_str() , sizeof(sendToPeer), 0);
        }

        return;
    }

    void LOGGED_IN::startListen(std::string IP)
    {
        // Child Process: peer-peer communication
        int peer_socket;
        peer_socket = socket(MY_NET_FAMILY, SOCK_STREAM, 0);

        // set up peer Port and IP
        struct sockaddr_in peer_address;
        peer_address.sin_family = MY_NET_FAMILY;
        peer_address.sin_addr.s_addr = inet_addr(IP.c_str());
        peer_address.sin_port = htons(port_num);

        // bind the peer socket
        bind(peer_socket, (struct sockaddr*) &peer_address, sizeof(peer_address));
        std::cout << "\nBind port #" << port_num << std::endl;

        while(true)
        {
            listen(peer_socket, 5);

            int to_peer_socket;
            to_peer_socket = accept(peer_socket, NULL, NULL);

            char peer_msg[] = "Hello, wanna date?\n";
            char recv_msg[100] = "";
            memset(recv_msg, '\0', sizeof(recv_msg));
            send(to_peer_socket, peer_msg, sizeof(peer_msg), 0);
            //std::cout << "IN\n";
            recv(to_peer_socket, &recv_msg, sizeof(recv_msg), 0);
            //std::cout << "OUT\n";
            std::cout << this->user_name << " received: " << recv_msg << std::flush;
            memset(recv_msg, '\0', sizeof(recv_msg));
            close(to_peer_socket);
        }
        std::cout << "child say goodbye~\n";
        exit(0);
    }
}
