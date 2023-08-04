#include "client.hpp"

Client::Client()
{
      this->username = "";
    this->nickname = "";
    this->real_name = "";
    this->isRegistred = false;
    this->user = false;
    this->nick = false;
    this->pass = false;  
    this->topicMode = false;
    this->kicked = false;
}

Client::~Client()
{

}

Client::Client(int socket_client)
{
    this->socket_client = socket_client;
    this->username = "";
    this->nickname = "";
    this->real_name = "";
    this->isRegistred = false;
    this->user = false;
    this->nick = false;
    this->pass = false;
    this->topicMode = false;
    this->kicked = false;
    this->newbuffer = "";
}

bool Client::get_topicMode()
{
    return (this->topicMode);
}

void Client::set_topicMode(bool topicMode)
{
    this->topicMode = topicMode;
}

int Client::get_socket_client() const
{
    return (this->socket_client);
}

std::string Client::get_username()
{
    return (this->username);
}

std::string Client::get_nickname() const
{
    return (this->nickname);
}

bool Client::get_isRegistred()
{
    return (this->isRegistred);
}

bool Client::get_user()
{
    return (this->user);
}

bool Client::get_kicked()
{
    return(this->kicked);
}

bool Client::get_nick()
{
    return (this->nick);
}

bool Client::get_pass()
{
    return (this->pass);
}

std::vector<std::string> Client::get_channels()
{
    return this->channels;
}

void Client::set_newbuffer(std::string buffer)
{
    this->newbuffer += buffer;
}

std::string Client::get_newbuffer()
{
    return this->newbuffer;
}

void Client::remove_channel(std::string channel)
{
    std::vector<std::string>::iterator it;
    for(it = this->channels.begin(); it != this->channels.end(); it++)
    {
        if(*it == channel)
        {
            this->channels.erase(it);
            break;
        }
    }
}

void Client::set_kicked(bool var)
{
    this->kicked = var;
}

void Client::set_user(std::string user, bool user_stat)
{
    this->username = user;
    this->user = user_stat;
}

void Client::set_nick(std::string nick, bool nick_stat)
{
    this->nickname = nick;
    this->nick = nick_stat;
}

void	Client::set_channel(std::string channel)
{
	this->channels.push_back(channel);
}

void Client::set_isRegistred()
{
    this->isRegistred = true;
}

void Client::set_pass(bool pass_stat)
{
    this->pass = pass_stat;
}

std::string Client::get_realname()
{
    return (this->real_name);
}

void Client::set_real_name(std::string rl_name)
{
    this->real_name = rl_name;
}

void Client::eraseBuffer()
{
    this->newbuffer.erase();
}
