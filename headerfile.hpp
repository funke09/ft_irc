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
#include <string.h>
#include <map>
#include "Message.hpp"

#define MAX_CLIENTS 100


class Server
{
    private:
    int port;
    int socket_fd;
    int client_fd;
    int flag;
    std::string password;
    struct sockaddr_in serverAddress;
    // struct sockaddr_in clientAddress;
    std::map<int, Message> clientMap;
    std::map<int, std::string> clients;
    struct pollfd fds[MAX_CLIENTS];


    public:
    Server(int port, std::string password);
    ~Server();
    int create_socket();
    int bind_socket();
    int listen_socket();
    void accept_socket();

    int get_port();
    int get_socket_fd();
    std::string get_password();
    void handel_message(char *buffer, int client_fd, int *fds, Message *user);
    
};


#endif