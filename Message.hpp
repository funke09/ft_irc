#ifndef MESSAGE_HPP
#define MESSAGE_HPP

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
#include <sstream>

#include "client.hpp"
#include "channel.hpp"
class Client;
class Channel;

class Message
{
    private:
    int socket;
    std::string message;
    std::string command;
    std::string prefix;
    std::vector<std::string> params;
    std::string trailing;
    bool is_authenticated;

    Client client;
    std::string C_password;



    public:
    Message();
    Message(int socket, std::string hostname);
    ~Message();
    int get_socket();
    Client &get_client();
    std::string parss_password(std::string password, std::string message, std::vector<Client> &clients);
    bool isNicknameTaken(const std::vector<Client>& clients, const std::string& nickname);   
};

void    erase_charcter(std::string& str, char c);
#endif