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



class Client
{
    private:
    std::string username;
    std::string nickname;
    std::string real_name;
    int socket_client;
    bool isRegistred;
    bool user;
    bool nick;
    bool pass;
    std::vector<std::string> channels;
    bool topicMode;
    bool kicked;

    std::string newbuffer;

    public:

    Client();
    ~Client();
    Client(int socket_client);
    int get_socket_client() const;
    std::string get_username();
    std::string get_nickname() const;
    std::string get_realname();
    bool get_isRegistred();
    bool get_user();
    bool get_nick();
    bool get_pass();
    void set_real_name(std::string rl_name);
    void set_isRegistred();
    void set_user(std::string user, bool user_stat);
    void set_nick(std::string nick, bool nick_stat);
    void set_pass(bool pass_stat);
    void set_channel(std::string channel);
    bool check_command(std::string Command);
    std::vector<std::string> get_channels();
    bool get_topicMode();
    void set_topicMode(bool topicMode);
    bool get_kicked();
    void set_kicked(bool var);
    void remove_channel(std::string channel);

    void set_newbuffer(std::string buffer);
    std::string get_newbuffer();
    void eraseBuffer();

};
bool operator==(const Client& lhs, const Client& rhs);

std::ostream& operator<<(std::ostream& os, Client& client);

#include "Message.hpp"

#endif



