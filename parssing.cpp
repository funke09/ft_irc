#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "headerfile.hpp"

int parss_args(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage: " << "<port>" << " <password>" << std::endl;
        return -1;
    }
    if(!isdigit(*av[1]) || atoi(av[1]) <= 6660 || atoi(av[1]) >= 6669)
    {
        std::cout << "Port must be a number or it's out of valid range for IRC ports" << std::endl;
        return -1;
    }
    if(strlen(av[2]) < 3)
    {
        std::cout << "Password must be at least 3 characters" << std::endl;
        return -1;
    }
    return 0;
}

int create_socket()
{
    int socket_fd;

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Failed to create socket" << std::endl;
        return -1;
    }
    int reuse = 1;
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == -1)
    {
        std::cout << "Failed to set socket options" << std::endl;
        close(socket_fd);
        return -1;
    }
    return (socket_fd);
}

int bind_socket(int port, int socket_fd)
{
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cout << "Failed to bind socket" << std::endl;
        close(socket_fd);
        return -1;
    } 
    return (0);
}

int main(int ac, char **av)
{

    if(parss_args(ac, av) == -1)
        return -1;
    Server server(atoi(av[1]), av[2]);
    if(server.create_socket() == -1)
        return -1;
    // ioctl();
    if(server.bind_socket() == -1)
        return -1;
    if(server.listen_socket() == -1)
        return -1;
    if(server.accept_socket() == -1)
        return -1;
}