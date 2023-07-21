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
#include <cctype>

#include "Message.hpp"
#include "client.hpp"
#include "channel.hpp"

#define MAX_CLIENTS 100

class Message;
class client;



class Server
{
    private:
    int port;
    int socket_fd;
    int client_fd;
    int flag;
    std::string password;
    struct sockaddr_in serverAddress;
    std::map<int, Message> clientMap;
    struct pollfd fds[MAX_CLIENTS];

    int _nbrchannels;

    // struct sockaddr_in clientAddress;
    // std::map<int, std::string> clients;
    std::vector<Client> clients;
    std::vector<Channel*> channels;
    std::vector<std::string>    _operators;

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
    void handel_message(char *buffer, Message *user);
    int existe(int fd);

    Channel *findChannel(std::string channelName) const;
    std::vector<std::string> SplitTargets(std::string token);
    void    JOIN(std::vector<std::string>& pars, Client *client);
    bool    isEmpty() const;
    void    setOperator(Client *client);
};


#endif