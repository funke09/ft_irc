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
#include <poll.h>
#include <sys/poll.h>

#define MAX_CLIENTS 100


class Server
{
    private:
    int port;
    int socket_fd;
    std::string password;
    struct sockaddr_in serverAddress;
    // struct sockaddr_in clientAddress;
    struct pollfd fds[MAX_CLIENTS];


    public:
    Server(int port, std::string password);
    ~Server();
    int create_socket();
    int bind_socket();
    int listen_socket();
    void accept_socket();


};

void handle_client(int client_fd);

#endif