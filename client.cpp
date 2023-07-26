#include "client.hpp"

Client::Client()
{

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
}

int Client::get_socket_client()
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

std::ostream& operator<<(std::ostream& os, Client& client)
{
    os << "Username: " << client.get_username() << std::endl;
    os << "Nickname: " << client.get_nickname() << std::endl;
    os << "Real Name: " << client.get_realname() << std::endl;
    os << "Socket Client: " << client.get_socket_client() << std::endl;
    os << "Is Registered: " << (client.get_isRegistred() ? "Yes" : "No") << std::endl;
    os << "User: " << (client.get_user() ? "Yes" : "No") << std::endl;
    os << "Nick: " << (client.get_nick() ? "Yes" : "No") << std::endl;
    os << "Pass: " << (client.get_pass() ? "Yes" : "No") << std::endl;
    // os << "Channels: ";
    // for (std::vector<std::string>::const_iterator it = client.channels.begin(); it != client.channels.end(); ++it)
    // {
    //     os << *it << " ";
    // }
    os << std::endl;

    return os;
}
bool Client::check_command(std::string Command)
{
   if (Command != "PASS" && Command != "NICK"  \
    && Command != "USER" && Command != "PRIVMSG" && Command != "NOTICE" && Command != "JOIN")
        return false;
    return true;
}

// bool operator==(const Client& lhs, const Client& rhs);
// {
//     return lhs.get_nickname() == rhs.get_nickname();
// }
