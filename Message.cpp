#include "Message.hpp"

Message::Message()
{
    // this->socket = 0;
    // this->params = std::vector<std::string>();
    // is_authenticated = false;
}


Message::Message(int socket)
{
    this->socket = socket;
    this->params = std::vector<std::string>();
    is_authenticated = false;
    this->client = Client(socket);
}

Message::~Message()
{
}

int Message::get_socket()
{
    return (this->socket);
}

std::string Message::get_prefix()
{
    return (this->prefix);
}

std::string Message::get_command()
{
    return (this->command);
}

std::vector<std::string> Message::get_params()
{
    return (this->params);
}

void Message::set_prefix(std::string prefix)
{
    this->prefix = prefix;
}

void Message::set_command(std::string command)
{
    this->command = command;
}

void Message::set_params(std::vector<std::string> params)
{
    this->params = params;
}
static std::vector<std::string> ft_split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

bool Message::isNicknameTaken(const std::vector<Client>& clients, const std::string& nickname) 
{
    for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) 
    {
        if (it->get_nickname() == nickname) 
            return true;  // Nickname already exists
    }
    return false;  // Nickname is available
}

void    erase_charcter(std::string& str, char c)
{
    size_t pos = str.find(c);
    while(pos != std::string::npos)
    {
        str.erase(pos, 1);
        pos = str.find(c);
    }
}

Client& Message::get_client()
{
    return this->client;
}


std::string Message::parss_password(std::string password, std::string buffer, std::vector<Client> &clients)
{
    std::vector<std::string> split;

    split = ft_split(buffer, ' ');
    if(!is_authenticated)
    {
        if(split[0] == "PASS" )
        {
            if(split.size() < 2)
                return (":localhost 461 * PASS :Not enough parameters\r\n");
            if(client.get_pass())
                return (":localhost 462 " + client.get_nickname() + " USER :You may not reregister\r\n");
            else if(split[1] == password)
            {
                client.set_pass(true);
                return ("");
            }
            else
            {
                return (":localhost 464 * PASS :Password incorrect\r\n");
            }
        }
        else if (split[0] == "USER" && client.get_pass())
        {
            if(split.size() < 5)
                return (":localhost 461 * USER :Not enough parameters\r\n");
            if (client.get_user())
                return (":localhost 462 " + client.get_username() + " USER :You may not reregister\r\n");
            else if ((split[2].size() == 1 && split[2][0] == '0') && (split[3].size() == 1 && split[3][0] == '*' ))
            {
                erase_charcter(split[4], ':');
                client.set_user(split[1], true);
                client.set_real_name(split[4]);
                // std::cout<<client<<std::endl;
                return ("");
            }
        }
        else if(split[0] == "NICK" && client.get_pass())
        {
            if (split.size() < 2)
                return (":localhost 431 * :No nickname given\r\n");
            else if (this->isNicknameTaken(clients, split[1]))
                return (":localhost 433 " + split[1] + " * :Nickname is already in use\r\n");
            else if ((split[1].size() > 50 || split[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]{}\\|") != std::string::npos))
                {
                    return (":localhost 432 * :Erroneous nickname\r\n");
                }
            else
            {
                std::string oldNickname = client.get_nickname();
                client.set_nick(split[1], true);
            }
        }
        if (client.get_pass() && client.get_nick() && !client.get_isRegistred())
        {
            std::string message = ":localhost 001 " + client.get_nickname() + " :Welcome to IRC Network " + client.get_nickname() + " \r\n";
            send(client.get_socket_client(), (char *)message.c_str(), message.length(), 0);
            message = ":localhost 002 " + client.get_nickname() + " :Your host is localhost, running version 1.0\r\n";
            send(client.get_socket_client(), (char *)message.c_str(), message.length(), 0);
            message = ":localhost 003 " + client.get_nickname() + " :This server was created 2023-03-25\r\n";
            send(client.get_socket_client(), (char *)message.c_str(), message.length(), 0);
            client.set_isRegistred();
            clients.push_back(client);
            return ("");
        }
    }
    return ("\r\n");
}



