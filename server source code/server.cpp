#include "server.h"

namespace CUSTOM
{
    Server::Server(int server_port, int pool_size): thread_pool(pool_size)
    {
        server_socket = socket(MY_NET_FAMILY, SOCK_STREAM, 0);
        // set up server Port and IP
        server_address.sin_family = MY_NET_FAMILY;
        server_address.sin_addr.s_addr = inet_addr(IP);
        if (server_port == -1) { // default port
            server_address.sin_port = htons(SERVER_PORT);
        }
        else { // custom port
            server_address.sin_port = htons(server_port);
        }

        // Initialize SSL Context text
        ctx = InitServerCTX();
        LoadCertificates(ctx, cert_file_path.c_str(), key_file_path.c_str());
    }

    void Server::Activate()
    {
        struct sockaddr_in client_address;

        // bind the server socket
        check(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)), "Bind Failed!\n");
        std::cout << "System Message: Bind port #" << SERVER_PORT << std::endl;

        check(listen(server_socket, SERVER_BACKLOG), "Listen Failed!\n");
        std::cout << "System Message: Listening...\n";

        // accept clients loop
        while(true)
        {
            int client_socket;
            struct sockaddr_in client_addrress;
            socklen_t client_addrress_size = sizeof(client_address);
            client_socket = accept(server_socket, (struct sockaddr*) &client_addrress, &client_addrress_size);
            if (check(client_socket, "Connection Failed") < 0)
            {
                continue;
            }

            // Add client_socket to secure SSL tunnel
            SSL* client_ssl = SSL_new(ctx);
            // Assign the socket into the SSL structure
            SSL_set_fd(client_ssl, client_socket);     

            // Perform SSL Handshake on the SSL server
            if (SSL_accept(client_ssl) == -1)
            {
                ERR_print_errors_fp(stderr);
                close(client_socket);
                continue;
            }
            ShowCerts(client_ssl);

            // https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
            // Parse IP address from sockaddr structure
            struct sockaddr_in *ipV4Addr = (struct sockaddr_in *)&client_addrress;
            struct in_addr ipAddr = ipV4Addr->sin_addr;
            char client_ip_addr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipAddr, client_ip_addr, INET_ADDRSTRLEN);

            std::string ip_str(client_ip_addr);
            CUSTOM::Client newClient(ip_str, client_socket, client_ssl);

            thread_pool.connect(newClient);
        }

        return;
    }

    // check whether a socket operation is fine
    int Server::check(int exp, const char *msg) {
        if (exp == SOCKETERROR) {
            perror(msg);
            exit(1);
        }
        return exp;
    }

    SSL_CTX* Server::InitServerCTX()
    {
        SSL_CTX *ctx;
        /* Initialize SSL library */
        SSL_library_init();
        /* Load all SSL Algos */
        OpenSSL_add_all_algorithms();
        /* Load all error strings */
        SSL_load_error_strings();
        /* 以 SSL V2 和 V3 標準相容方式產生一個 SSL_CTX ，即 SSL Content Text */
        ctx = SSL_CTX_new(SSLv23_server_method());
        /* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 單獨表示 V2 或 V3標準 */
        if (ctx == NULL)
        {
            ERR_print_errors_fp(stdout);
            abort();
        }
        return ctx;
    }

    void Server::LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile)
    {
        /* 載入使用者的數字證書， 此證書用來發送給客戶端。 證書裡包含有公鑰 */
        if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
        {
            ERR_print_errors_fp(stderr);
            abort();
        }
        /* 載入使用者私鑰 */
        if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
        {
            ERR_print_errors_fp(stderr);
            abort();
        }
        /* 檢查使用者私鑰是否正確 */
        if ( !SSL_CTX_check_private_key(ctx) )
        {
            fprintf(stderr, "Private key does not match the public certificate\n");
            abort();
        }
    }

    void Server::ShowCerts(SSL *ssl)
    {
        X509 *cert;
        char *line;

        cert = SSL_get_peer_certificate(ssl);
        if (cert != NULL)
        {
            printf("Digital certificate information:\n");
            line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
            printf("Certificate: %s\n", line);
            free(line);
            line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
            printf("Issuer: %s\n", line);
            free(line);
            X509_free(cert);
        }
        else
            printf("No certificate information！\n");
    }
}