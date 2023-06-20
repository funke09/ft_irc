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
    if (ac != 2)
    {
        std::cout << "Usage: " << "<port>" << " <password>" << std::endl;
        return -1;
    }
    if(!isdigit(*av[1]) || atoi(av[1]) < 1 || atoi(av[1]) > 65535)
    {
        std::cout << "Port must be a number or it's out of valid range for TCP/UDP ports" << std::endl;
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
    
}

int main(int ac, char **av)
{
    if(parss_args(ac, av) == -1)
        return -1;
    if(create_socket() == -1)
        return -1;
}