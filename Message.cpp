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

std::string Message::parss_password(std::string password, std::string buffer, std::vector<Client> &clients)
{
    message = buffer;
    std::cout << "this->socket = " <<this->get_socket() << std::endl;
    if(!is_authenticated)
    {
        std::string sen = "enter password";
        int bit = send(this->socket, sen.c_str(), sen.length(), 0);
        if(bit == -1)
        {
            std::cout<<"error in send"<<std::endl;
        }
        if(message.substr(0, 4) == "PASS")
        {
            if(client.get_pass())
                return (":localhost 462 USER :You may not reregister\r\n");
            else if(message.substr(5, password.length()) == password)
            {
                client.set_pass(true);
                return (":localhost 001 * :Welcome to the Internet Relay Network\r\n");
            }
            else if(message.empty())
            {
                return (":localhost 461 * PASS :Not enough parameters\r\n");
            }
            else
            {
                return (":localhost 464 * PASS :Password incorrect\r\n");
            }
        }
        else if (message.substr(0, 4) == "USER" && client.get_pass())
        {
            std::vector<std::string> split;
            split = ft_split(message, ' ');

            if (client.get_user())
                return (":localhost 462 USER :You may not reregister\r\n");
            else if(split.size() < 5)
                return (":localhost 461 * USER :Not enough parameters\r\n");
            else if ((split[2].size() == 1 && split[2][0] == '0') && (split[3].size() == 1 && split[3][0] == '*') && split[4][0] == ':')
            {
                // params.push_back(message.substr(0, pos));
                client.set_user(split[1], true);
                client.set_real_name(split[4].substr(1));
                std::cout<<client<<std::endl;
                clients.push_back(client);
                return ("");
            }
        }
        else if(message.substr(0, 4) == "NICK")
        {
            std::vector<std::string> split;
            split = ft_split(message, ' ');

        }
        else
        {
            return ("Not authenticated");
        }

    }
    
    return (":localhost 462 " /*+ client.get_nick()*/   " PASS :You may not reregister\r\n");
// std::string Message::parss_password(std::string password, std::string buffer)
// {
//     message = buffer;
//     if(this->message[0] == ':')
//     {
//         int pos = this->message.find(" ");
//         this->prefix = this->message.substr(1, pos - 1);
//         this->message = this->message.substr(pos + 1);
//     }
//     int pos = this->message.find(" ");
//     this->command = this->message.substr(0, pos);
//     this->message = this->message.substr(pos + 1);
//     if(pos == std::string::npos)
//     {
//         if(this->command == "PASS")
//         {
            
//         }
//         else if(this->command == "NICK")
//         {
         
//         }
//         else if(this->command == "USER")
//         {
         
//         }
//     }

//     return (":localhost 462 " /*+ client.get_nick()*/   " PASS :You may not reregister\r\n");

}
