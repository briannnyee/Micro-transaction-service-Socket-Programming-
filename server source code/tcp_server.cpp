#include "server.h"

// Server main function
int main()
{
    // Enter port to listen
    int server_port = -1;
    std::cout << "Enter port for server to listen (-1 for default port): ";
    std::cin >> server_port;

    // Initialize server and the thread pool
    CUSTOM::Server server(server_port, THREAD_POOL_SIZE);

    // Activates server, let the party begin
    server.Activate();

    return 0;
}