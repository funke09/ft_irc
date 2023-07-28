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
#include <netdb.h>


#include "Message.hpp"
#include "client.hpp"
#include "channel.hpp"

#define MAX_CLIENTS 100

class Message;
class Client;
class Channel;



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
    
    // struct sockaddr_in clientAddress;
    // std::map<int, std::string> clients;
    std::vector<Client> clients;
    std::vector<Channel> _channels;
    // Client client;

    public:
    Server();
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
    std::string send_intro_message();
    std::string privmsg(std::string buff, Client &client);
    std::string 	mode_response(std::vector<std::string> split, Client &client);
    std::string joinChannel(std::vector<std::string> pars, Client& client);
    std::string ft_time(void);
    int getChannel(std::string channelName);
    bool isOnChannel(const std::vector<std::string>& channels, const std::string& input);
    bool channelExists(std::vector<Channel> channels, std::string& targetName);
    std::vector<Channel> getChannelsVector();
    bool OnChannel(const std::vector<std::string>& channels, const std::string& input);
    std::string parss_topic(std::string buffer , Client &client);

};

int findClientSocket(const std::vector<Client>& clients, const Client& targetClient);
std::string get_adderss();

#endif