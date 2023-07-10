#ifndef CLIENT_HPP
#define CLIENT_HPP

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
#include <cctype>


#include "Message.hpp"

class Client
{
    private:
    std::string username;
    std::string nickname;
    int socket_client;
    bool isRegistred;
    bool user;
    bool nick;
    bool pass;

    public:

    Client();
    ~Client();
    Client(int socket_client, std::string username, std::string nickname);
    int get_socket_client();
    std::string get_username();
    std::string get_nickname();
    bool get_isRegistred();
    bool get_user();
    bool get_nick();
    bool get_pass();
};

#endif


