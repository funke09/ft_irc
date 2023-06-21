#ifndef HEADERFILE_HPP
#define HEADERFILE_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 100


class Server
{
    private:
    int port;
    int socket_fd;
    // std::vector<Client> clients;
    // std::vector<Channel> channels;
    std::string password;
    // struct sockaddr_in serverAddress;

    public:
    Server(int port, std::string password);
    ~Server();
    int create_socket();
    int bind_socket();
    int listen_socket();
    int accept_socket();
};


#endif