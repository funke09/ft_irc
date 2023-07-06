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
    std::cout << "Server started and listening on port " << this->port << std::endl;
    return (0);
}


int Server::accept_socket()
{
    fds.push_back((struct pollfd){.fd = socket_fd, .events = POLLIN, .revents = 0});

    while (1)
    {
        poll(fds.data(), fds.size(), -1);
        for(size_t i = 0; i < fds.size(); i++)
        {
            if(fds[i].revents & POLLIN)
            {
                if(fds[i].fd == socket_fd)
                {
                    // struct sockaddr_in clientAddress;
                    // socklen_t clientAddressLength = sizeof(clientAddress);
                    // int client_fd;
                    // if((client_fd = accept(socket_fd, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)
                    // {
                    //     std::cout << "Failed to accept connection" << std::endl;
                    //     close(socket_fd);
                    //     return -1;
                    // }
                    // fds.push_back((struct pollfd){.fd = client_fd, .events = POLLIN, .revents = 0});
                    // std::cout << "Client connected" << std::endl;
                }
                else
                {
                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(fds[i].fd, buffer, 1024, 0);
                    if(bytes <= 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                    }
                    else
                    {
                        std::cout << "Received " << bytes << " bytes" << std::endl;
                        std::cout << buffer << std::endl;
                    }
                }
            }
        }
    }
    
    // int client_fd;
    // socklen_t clientAddressLength = sizeof(clientAddress);

    // if((client_fd = accept(this->socket_fd, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)
    // {
    //     std::cout << "Failed to accept connection" << std::endl;
    //     close(this->socket_fd);
    //     return -1;
    // }
    // std::cout << "Client connected" << std::endl;
    // // handle_client(client_fd);

    // return (client_fd);


}
