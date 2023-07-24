#include "headerfile.hpp"
#include "channel.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    // std::cout << "from here "<<this->password << std::endl;
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


void removeNewline(std::string& str) {
    if (!str.empty() && str[str.length() - 1] == '\n') {
        str.erase(str.length() - 1);
    }
}

int Server::existe(int fd)
{
 
    std::vector<Client>::iterator it;
    for(it = clients.begin(); it != clients.end(); it++)
    {
       if(it->get_socket_client() == fd)
       {
            std::cout << "existe" << std::endl;
           return (1);
       }
    }
    std::cout << "not existe" << std::endl;
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
     // Map to store client information
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

        for (int j = 0; j < fds_num; j++) {
            if (fds[j].revents & POLLIN) {
                if (fds[j].fd == this->socket_fd) {
                    // Accept new client connection
                    if ((client_fd = accept(this->socket_fd, (struct sockaddr*)&serverAddress, (socklen_t*)&i)) == -1) {
                        std::cout << "Failed to accept connection" << std::endl;
                        close(this->socket_fd);
                        return;
                    }

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
                        std::cout << "Client connected" << std::endl;
                    }
                }
                else {
                    // int clientfd = fds[j].fd;
                    flag = j - 1;
                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(fds[j].fd, buffer, 1024, 0);
 
                    if (bytes < 0) {
                        // Client disconnected
                        std::cout << "Client disconnected" << std::endl;
                        close(fds[j].fd);
                        fds[j].fd = -1;

                        // // Remove client from the map
                        // clientMap.erase(fds[j].fd);
                    }
                    if(bytes == 0 || !strcmp(buffer, "QUIT leaving...\r\n"))
                    {
                        fds_num--;
                        close(fds[j].fd);
                        fds[j].fd = -1;
                        std::cout << "Client disconnected" << std::endl;
                    }
                    
                    else if (!existe(fds[j].fd) && buffer[0] != '\n')
                    {
                        handel_message(buffer, &clientMap[flag]);
                    }
                }
            }
        }
    }
}

// std::string Server::send_intro_message()
// {
//     std::string welcome_message;
//     welcome_message = ": 001 Welcome to the Internet Relay " + this + "\r\n";
//     return (welcome_message);
// }

void Server::handel_message(char *buff, Message *user)
{
    std::string buffer(buff);
    std::string response = "";
    std::vector<std::string> input;

    erase_charcter(buffer, '\n');
    erase_charcter(buffer, '\r');
    input = ft_split(buffer, ' ');
    response = user->parss_password(password, buffer, this->clients);
    // if(client.get_isRegistred())
    //     response = send_intro_message();
    if(input[0] == "JOIN")
        std::cout<<"ehehehehe:"<<response<<std::endl;
    // else if(input[0] == "TOPIC")
    //     response = chan->parss_topic(buffer);
    else if(input[0] == "MODE")
        response = mode_response(input, this->clients[user->get_socket()]);
    std::cout<<response<<std::endl;
    int bit = send(this->client_fd, response.c_str(), response.length(), 0);
    if(bit == -1)
    {
        std::cout<<"error in send"<<std::endl;
    }

}

// void send_to_client(int numofmessage, int client_fd, std::string response)
// {
    
// }