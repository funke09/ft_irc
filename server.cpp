#include "headerfile.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
}

Server::~Server()
{
    close(this->socket_fd);
}

int Server::create_socket()
{
    if((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Failed to create socket" << std::endl;
        return -1;
    }
    int reuse = 1;
    if(setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == -1)
    {
        std::cout << "Failed to set socket options" << std::endl;
        close(this->socket_fd);
        return -1;
    }
    return (0);
}

int Server::bind_socket()
{
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cout << "Failed to bind socket" << std::endl;
        close(this->socket_fd);
        return -1;
    } 
    return (0);
}

int Server::listen_socket()
{
    if(listen(this->socket_fd, MAX_CLIENTS) == -1)
    {
        std::cout << "Failed to listen for connections" << std::endl;
        close(this->socket_fd);
        return -1;
    }
    return (0);
}

int Server::accept_socket()
{
    int client_fd;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    if((client_fd = accept(this->socket_fd, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)
    {
        std::cout << "Failed to accept connection" << std::endl;
        close(this->socket_fd);
        return -1;
    }
    return (client_fd);
}

