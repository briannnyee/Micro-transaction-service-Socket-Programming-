#include "tcp_client_FSM.h"
#include "ssl_for_client.h"

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

            int port_num;
            std::cout << "Port Number: ";
            std::cin >> port_num;
            std::cin.ignore(MAX_STRING_LENGTH, '\n');
            while (port_num < 1024 || port_num > 65535)
            {
                std::cout << "The port number should be in range 1024 ~ 65535!\n\n";
                std::cout << "Port Number: ";
                std::cin >> port_num;
                std::cin.ignore(MAX_STRING_LENGTH, '\n');
            }

            printSeperator();

            sendToServer = user_name + "#" + std::to_string(port_num);// + CRLF;
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
        activated_msg = "Hello " + user_name + ",how may I serve you today?\n";
        state_feature.push_back("Check Balances");
        state_feature.push_back("Microtransaction");
        state_feature.push_back("Logout");
    }

    std::string LOGGED_IN::microTransaction(std::string to_user, std::string amount, std::string ip_to_send, int port_to_send)
    {
        // Initialize SSL Context text
        SSL_CTX* microTran_ctx = InitClientCTX();
        LoadCertificates(microTran_ctx);

        // SSL socket
        SSL* to_peer_ssl = nullptr;

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
            // Connect microTran_ctx and SSL
            to_peer_ssl = SSL_new(microTran_ctx);
            SSL_set_fd(to_peer_ssl, to_peer_socket);
            // Establish SSL connection
            if (SSL_connect(to_peer_ssl) == -1)
            {
                ERR_print_errors_fp(stderr);
                std::cout << "SSL connection failed!\n";
                return "";
            }
            else
            {
                printf("Connected with %s encryption\n", SSL_get_cipher(to_peer_ssl));
                printSeperator();
                std::cout << "Certificate\n";
                ShowCerts(to_peer_ssl);
                printSeperator();
            }
            // receive data from the peer
            SSL_read(to_peer_ssl, &peer_response, sizeof(peer_response));
            std::cout << user_name << " received: " << peer_response << std::endl;
            memset(peer_response, '\0', sizeof(peer_response));
            std::string sendToPeer = user_name + "#" + amount + "#" + to_user + "&";
            SSL_write(to_peer_ssl, sendToPeer.c_str() , sizeof(sendToPeer));
        }

        SSL_shutdown(to_peer_ssl);
        SSL_free(to_peer_ssl);
        SSL_CTX_free(microTran_ctx);
        close(to_peer_socket);

        return user_name + "#" + amount + "#" + to_user + "&";
    }

    void LOGGED_IN::startListen(std::string IP, SSL* server_ssl)
    {
        SSL_CTX* child_ctx = InitServerCTX();
        LoadCertificates(child_ctx);

        SSL* B2A_ssl = nullptr;

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

        listen(peer_socket, 5);


        while(true)
        {
            int to_peer_socket;
            to_peer_socket = accept(peer_socket, NULL, NULL);
            if (to_peer_socket < 0)
            {
                perror("ERROR on accept");
            }
            else
            {
                /* 將連線使用者的 socket 加入到 SSL */
                B2A_ssl = SSL_new(child_ctx);
                SSL_set_fd(B2A_ssl, to_peer_socket);
                /* 建立 SSL 連線 */
                if (SSL_accept(B2A_ssl) == -1)
                {
                    ERR_print_errors_fp(stderr);
                    close(to_peer_socket);
                    continue;
                }
                std::cout << std::endl;
                printSeperator();
                std::cout << "Certificate\n";
                ShowCerts(B2A_ssl);
                printSeperator();
            }

            char peer_msg[] = "Hello, how are you?\n";
            char recv_msg[MAX_STRING_LENGTH] = "";
            memset(recv_msg, '\0', sizeof(recv_msg));
            SSL_write(B2A_ssl, peer_msg, sizeof(peer_msg));
            SSL_read(B2A_ssl, &recv_msg, sizeof(recv_msg));
            std::cout << this->user_name << " Received: " << recv_msg << std::flush;
            std::cout << std::endl;

            // // send transaction message to server
            // SSL_write(server_ssl, recv_msg, sizeof(recv_msg));
            // memset(recv_msg, '\0', sizeof(recv_msg));
            // SSL_read(server_ssl, &recv_msg, sizeof(recv_msg));
            // std::cout << "Server Re: " << recv_msg << std::flush;
            // printSeperator();
            memset(recv_msg, '\0', sizeof(recv_msg));

            std::cout << "Select: ";

            /* 關閉 SSL 連線 */
            SSL_shutdown(B2A_ssl);
            /* 釋放 SSL */
            SSL_free(B2A_ssl);
            close(to_peer_socket);
        }
        close(peer_socket);
        std::cout << "child say goodbye~\n";
        return;
    }
}
