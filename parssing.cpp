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


int main(int ac, char **av)
{

    if(parss_args(ac, av) == -1)
        return -1;
    Server server(atoi(av[1]), av[2]);
    if(server.create_socket() == -1)
        return -1;
    if(server.bind_socket() == -1)
        return -1;
    if(server.listen_socket() == -1)
        return -1;
    server.accept_socket();
}