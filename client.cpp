#include "client.hpp"

Client::Client()
{

}

Client::~Client()
{

}

Client::Client(int socket_client, std::string username, std::string nickname)
{
    this->socket_client = socket_client;
    this->username = username;
    this->nickname = nickname;
    this->isRegistred = false;
}

int Client::get_socket_client()
{
    return (this->socket_client);
}

std::string Client::get_username()
{
    return (this->username);
}

std::string Client::get_nickname()
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

bool Client::get_nick()
{
    return (this->nick);
}

bool Client::get_pass()
{
    return (this->pass);
}

