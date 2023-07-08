#include "headerfile.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    // std::cout << "from here "<<this->password << std::endl;
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

void Server::accept_socket(void) {
    int fds_num;
    int socket;
    size_t i;
    std::map<int, std::string> clientMap; // Map to store client information
    std::vector<std::string> Channels;

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
                    if ((socket = accept(this->socket_fd, (struct sockaddr*)&serverAddress, (socklen_t*)&i)) == -1) {
                        std::cout << "Failed to accept connection" << std::endl;
                        close(this->socket_fd);
                        return;
                    }

                    // Check if maximum number of clients reached
                    if (fds_num == MAX_CLIENTS) {
                        std::cout << "Maximum number of clients reached" << std::endl;
                        close(socket);
                    }
                    else {
                        fds[fds_num].fd = socket;
                        fds[fds_num].events = POLLIN;
                        fds_num++;
                        std::cout << "Client connected" << std::endl;
                    }
                }
                else {
                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(fds[j].fd, buffer, 1024, 0);
                    if (bytes <= 0) {
                        // Client disconnected
                        std::cout << "Client disconnected" << std::endl;
                        close(fds[j].fd);
                        fds[j].fd = -1;

                        // Remove client from the map
                        clientMap.erase(fds[j].fd);
                    }
                    else {
                        std::cout << "Received " << bytes << " bytes" << std::endl;
                        std::cout << "from client: " << buffer << std::endl;

                        if (clientMap.find(fds[j].fd) == clientMap.end()) {
                            // New client, expecting password
                            if (strncmp(buffer, "PASS ", 5) == 0) {
                                std::string pass = std::string(buffer).substr(5);
                                removeNewline(pass);
                            
                                // std::cout << "Client pass: " << pass << "len :" << pass.length() << std::endl;
                                // std::cout << "MY password: " << this->get_password() << "len :"<< password.length() << std::endl;
                                if (pass != password) {
                                    std::cout << "Incorrect password. Closing connection." << std::endl;
                                    close(fds[j].fd);
                                }
                                else {
                                    std::cout << "Client authenticated" << std::endl;
                                    // Add client to the map with an empty username and nickname
                                    clientMap[fds[j].fd] = "";
                                }
                            }
                            else {
                                std::cout << "Invalid password format. Closing connection." << std::endl;
                                close(fds[j].fd);
                            }
                        }
                        else {
                            // Existing client, expecting client information
                            std::string clientInfo(buffer);

                            // Parse client information (username, nickname, password)
                            // Assuming the format: "USERNAME <username> NICKNAME <nickname> PASSWORD <password>"
                            std::string username, nickname, password, channel;
                            size_t pos;

                            pos = clientInfo.find("USERNAME ");
                            if (pos != std::string::npos) {
                                pos += 9; // Length of "USERNAME "
                                size_t endPos = clientInfo.find(" ", pos);
                                if (endPos != std::string::npos) {
                                    username = clientInfo.substr(pos, endPos - pos);
                                    pos = endPos + 1;
                                }
                            }

                            pos = clientInfo.find("NICKNAME ", pos);
                            if (pos != std::string::npos) {
                                pos += 9; // Length of "NICKNAME "
                                size_t endPos = clientInfo.find(" ", pos);
                                if (endPos != std::string::npos) {
                                    nickname = clientInfo.substr(pos, endPos - pos);
                                    pos = endPos + 1;
                                }
                            }

                            pos = clientInfo.find("PASSWORD ", pos);
                            if (pos != std::string::npos) {
                                pos += 9; // Length of "PASSWORD "
                                password = clientInfo.substr(pos);
                            }

                            // Verify client information
                            if (username.empty() || nickname.empty() || password.empty()) {
                                std::cout << "Invalid client information. Closing connection." << std::endl;
                                close(fds[j].fd);
                                clientMap.erase(fds[j].fd);
                            }
                            
                            pos = clientInfo.find("#", pos);
                            if (pos != std::string::npos) {
                                pos += 1;
                                for (unsigned long i = 0; i < Channels.size(); i++)
                                {
                                    if (Channels[i] == clientInfo.substr(pos)) {
                                        std::cout << "Channel already exists" << std::endl;
                                        break;
                                    }
                                    else {
                                        std::cout << "Channel created" << std::endl;
                                        Channels.push_back(clientInfo.substr(pos));
                                        channel = clientInfo.substr(pos);
                                        break;
                                    }
                                }
                                for (unsigned long i = 0; i < Channels.size(); i++)
                                {
                                    std::cout << Channels[i] << std::endl;
                                }
                                
                            }
                            else {
                                // Store client information in the map
                                clientMap[fds[j].fd] = "Username: " + username + ", Nickname: " + nickname + ", Password: " + password + ", Channel: " + channel;

                                // Perform further actions with the client information
                                std::cout << "Client information: " << clientMap[fds[j].fd] << std::endl;


                            }
                        }
                    }
                }
            }
        }
    }
}
