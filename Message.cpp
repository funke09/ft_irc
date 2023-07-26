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


std::string Message::parss_password(std::string password, std::string buffer, std::vector<Client> &clients)
{
    std::vector<std::string> split;
    Server server;

    split = ft_split(buffer, ' ');
    if(!is_authenticated)
    {
        if(split[0] == "PASS")
        {
            if(client.get_pass())
                return (":localhost 462 " + client.get_nickname() + " USER :You may not reregister\r\n");
            else if(split[1].empty())
            {
                return (":localhost 461 * PASS :Not enough parameters\r\n");
            }
            else if(split[1] == password)
            {
                client.set_pass(true);
                return ("PASSWEORED VALID\r\n");
            }
            else
            {
                return (":localhost 464 * PASS :Password incorrect\r\n");
            }
        }
        else if (split[0] == "USER" && client.get_pass())
        {
            if (client.get_user())
                return (":localhost 462 " + client.get_username() + " USER :You may not reregister\r\n");
            else if(split.size() < 5)
                return (":localhost 461 * USER :Not enough parameters\r\n");
            else if ((split[2].size() == 1 && split[2][0] == '0') && (split[3].size() == 1 && split[3][0] == '*') )
            {
                erase_charcter(split[4], ':');
                client.set_user(split[1], true);
                client.set_real_name(split[4]);
                return ("");
            }
        }
        else if(split[0] == "NICK" && client.get_pass() && client.get_user())
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
                client.set_isRegistred();
    
                // Broadcast the nickname change to other clients
                // ... send the nicknameChangeMessage to other clients ...
            }
            if (client.get_isRegistred())
            {   std::cout << "client: " << client << std::endl;
                clients.push_back(client);
                std::string nickename = client.get_nickname();
                return (":localhost 001 * :Welcome to the Internet Relay Network " + nickename + "\r\n");
            }
        }
    }
    return ("Not authenticated\r\n");
}



