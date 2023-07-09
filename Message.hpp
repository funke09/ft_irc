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

class Message
{
    private:
    int socket;
    std::string message;
    std::string command;
    std::string prefix;
    std::vector<std::string> params;
    std::string trailing;


    public:
    Message(int socket);
    ~Message();
    std::string get_message();
    std::string get_command();
    std::string get_prefix();
    std::vector<std::string> get_params();
    std::string get_trailing();
    void set_message(std::string message);
    void set_command(std::string command);
    void set_prefix(std::string prefix);
    void set_params(std::vector<std::string> params);
    void set_trailing(std::string trailing);
    int parse_message(std::string password, std::string message);
    void print_message();
    void print_params();
    void print_prefix();
    void print_command();
    void print_trailing();
    void print_all();

};

#endif