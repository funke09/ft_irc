#include "headerfile.hpp"
#include "channel.hpp"



Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
}

Server::Server()
{
}

int Server::get_port()
{
    return (this->port);
}

int Server::get_socket_fd()
{
    return (this->socket_fd);
}

std::string Server::get_password()
{
    return (this->password);
}

std::vector<Channel> Server::getChannelsVector()
{
    if(_channels.empty())
    {
        std::cout << "channels is empty" << std::endl;
    }
    else
    {
        std::cout << "channels is not empty" << std::endl;
    }
    std::vector<Channel>::iterator it;
    for(it = _channels.begin(); it != _channels.end(); it++)
    {
        std::cout << "in get vector : " << it->getName() << std::endl;
    }
    return (this->_channels);
}

Server::~Server()
{
    close(this->socket_fd);
}

int Server::create_socket()
{
    if((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Failed to create socket" << std::endl;
        return -1;
    }
    int reuse = 1;
    if(setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == -1)
    {
        std::cout << "Failed to set socket options" << std::endl;
        close(this->socket_fd);
        return -1;
    }
    return (0);
}

int Server::bind_socket()
{
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cout << "Failed to bind socket" << std::endl;
        close(this->socket_fd);
        return -1;
    } 
    return (0);
}

int Server::listen_socket()
{
    if(listen(this->socket_fd, MAX_CLIENTS) == -1)
    {
        std::cout << "Failed to listen for connections" << std::endl;
        close(this->socket_fd);
        return -1;
    }
    std::cout << "Server started and listening on port " << this->port << std::endl;
    return (0);
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


void Server::accept_socket(void) {
    int fds_num;
    size_t i;
    std::vector<std::string> Channels;
    int user_id = 0;

    i = sizeof(serverAddress);
    memset(fds, 0, MAX_CLIENTS * sizeof(struct pollfd));
    fds[0].fd = this->socket_fd;
    fds[0].events = POLLIN;
    fds_num = 1;

    while (1) {
        if (poll(fds, fds_num, -1) == -1) {
            std::cout << "Failed to poll" << std::endl;
            close(this->socket_fd);
            return;
        }
        for (int j = 0; j < fds_num; j++) 
        {
            if (fds[j].revents & POLLIN) 
            {
                if (fds[j].fd == this->socket_fd) 
                {
                    // Accept new client connection
                    if ((client_fd = accept(this->socket_fd, (struct sockaddr*)&serverAddress, (socklen_t*)&i)) == -1) {
                        std::cout << "Failed to accept connection" << std::endl;
                        close(this->socket_fd);
                        return;
                    }
                    fcntl(client_fd, F_SETFL, O_NONBLOCK);

                    // Check if maximum number of clients reached
                    if (fds_num == MAX_CLIENTS) {
                        std::cout << "Maximum number of clients reached" << std::endl;
                        close(client_fd);
                    }
                    else {
                        fds[fds_num].fd = client_fd;
                        fds[fds_num].events = POLLIN;
                        fds_num++;
                        Message msg(client_fd);
                        this->clientMap[user_id] = msg;
                        user_id++;
                        std::cout << "Connection accepted, Client trying to connect..." << std::endl;
                    }
                }
                else {
                    flag = j - 1;
                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(fds[j].fd, buffer, 1024, 0);
                    if (bytes == 0 || !strcmp(buffer, "QUIT\r\n") || !strcmp(buffer, "QUIT\n")) 
                    {
                        fds[j].events = 0;
                        std::vector<Client>::iterator it;
                        if(clients.size())
                        {
                            for(it = clients.begin(); it < clients.end(); it++)
                            {
                                if(it->get_socket_client() == fds[j].fd) 
                                {
                                    it->eraseBuffer();
                                    clients.erase(it);
                                }
                            }

                        }
                        std::vector<Channel>::iterator it1;
                        if(_channels.size())
                        {
                            for(it1 = _channels.begin(); it1 < _channels.end(); it1++)
                            {
                                if(it1->is_moderator(fds[j].fd))
                                    it1->erase_moderator(fds[j].fd);
                                if (it1->is_member(fds[j].fd))
                                    it1->erase_member(fds[j].fd);
                                if (it1->getMembers().empty())
                                    _channels.erase(it1);
                            }
                        }
                        close(fds[j].fd);
                        std::cout << "client disconnect" << std::endl;
                    }
                     if(bytes < 0)
                    {
                        fds_num--;
                        close(fds[j].fd);
                        fds[j].fd = -1;
                        std::cout << "nothing to read" << std::endl;
                    }
                    else if (buffer[0] != '\n')
                    {
                        handel_message(buffer, &clientMap[flag]);
                    }
                }
            }
        }
    }
}


std::string toUpperCase(const std::string& str) {
    std::string result = str;

    for (size_t i = 0; i < str.length(); ++i) {
        result[i] = std::toupper(str[i]);
    }

    return result;
}


void Server::handel_message(char *buff, Message *user)
{
    std::string buffer;
    bool new_line = false;
    std::string response = "";
    std::vector<std::string> input;
    for(int i = 0; buff[i] != '\0'; i++)
    {
        if(buff[i] == '\n')
        {
            new_line = true;
            break;
        }
    }
    if(new_line == false)
    {
        user->get_client().set_newbuffer(buff);
        return;
    }
    else
    {
        user->get_client().set_newbuffer(buff);
        buffer = user->get_client().get_newbuffer();
        user->get_client().eraseBuffer();
    }
    std::string line = buffer;
    erase_charcter(line, '\n');
    erase_charcter(line, '\r');
    input = ft_split(line, ' ');
    response = user->parss_password(password, line, this->clients);
    if(toUpperCase(input[0]) == "JOIN")
        response = joinChannel(input, user->get_client());
    else if(toUpperCase(input[0]) == "MODE")
        response = mode_response(input, user->get_client()); 
    else if(toUpperCase(input[0]) == "PRIVMSG")
        response = privmsg(line, user->get_client());
    else if(toUpperCase(input[0]) == "TOPIC")
        response = parss_topic(line, user->get_client());
    else if(toUpperCase(input[0]) == "KICK")
        response = kick(line, user->get_client());
    else if(toUpperCase(input[0]) == "INVITE")
        response = invite(input, user->get_client());
    else if(toUpperCase(input[0]) == "/BOT")
        response = bot(user->get_client());
    if (response.length())
    {
        int bit = send(user->get_client().get_socket_client(), response.c_str(), response.length(), 0);
        if(bit == -1)
        {
            std::cout<<"error in send"<<std::endl;
        }
    }

}
