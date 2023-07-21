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
class Client;

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
    Message(int socket);
    ~Message();
    std::string get_message();
    std::string get_command();
    std::string get_prefix();
    int get_socket();
    std::vector<std::string> get_params();
    std::string get_trailing();
    void set_message(std::string message);
    void set_command(std::string command);
    void set_prefix(std::string prefix);
    void set_params(std::vector<std::string> params);
    void set_trailing(std::string trailing);
    std::string parss_password(std::string password, std::string message, std::vector<Client> &clients);
    bool isNicknameTaken(const std::vector<Client>& clients, const std::string& nickname);
    // void print_message();
    // void print_params();
    // void print_prefix();
    // void print_command();
    // void print_trailing();
    // void print_all();
    void check_command(std::string message, std::string command, std::string password, int size);
    void handleError(int error);
    Client& getClient();
    
};

void    erase_charcter(std::string& str, char c);
#endif