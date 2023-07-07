#include "headerfile.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
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


// int Server::accept_socket()
// {
//     fds.push_back((struct pollfd){.fd = socket_fd, .events = POLLIN, .revents = 0});

//     while (1)
//     {
//         poll(fds.data(), fds.size(), -1);
//         for(size_t i = 0; i < fds.size(); i++)
//         {
//             if(fds[i].revents & POLLIN)
//             {
//                 if(fds[i].fd == socket_fd)
//                 {

//                 }
//                     // struct sockaddr_in clientAddress;
//                     // socklen_t clientAddressLength = sizeof(clientAddress);
//                     // int client_fd;
//                     // if((client_fd = accept(socket_fd, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)
//                     // {
//                     //     std::cout << "Failed to accept connection" << std::endl;
//                     //     close(socket_fd);
//                     //     return -1;
//                     // }
//                     // fds.push_back((struct pollfd){.fd = client_fd, .events = POLLIN, .revents = 0});
//                     // std::cout << "Client connected" << std::endl;
//                 else
//                 {
//                     char buffer[1024];
//                     memset(buffer, 0, 1024);
//                     int bytes = recv(fds[i].fd, buffer, 1024, 0);
//                     if(bytes <= 0)
//                     {
//                         std::cout << "Client disconnected" << std::endl;
//                         close(fds[i].fd);
//                         fds.erase(fds.begin() + i);
//                     }
//                     else
//                     {
//                         std::cout << "Received " << bytes << " bytes" << std::endl;
//                         std::cout << buffer << std::endl;
//                     }
//                 }
//             }
//         }
//     }
    
//     // int client_fd;
//     // socklen_t clientAddressLength = sizeof(clientAddress);

//     // if((client_fd = accept(this->socket_fd, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)
//     // {
//     //     std::cout << "Failed to accept connection" << std::endl;
//     //     close(this->socket_fd);
//     //     return -1;
//     // }
//     // std::cout << "Client connected" << std::endl;
//     // // handle_client(client_fd);

//     // return (client_fd);


// }

void Server::accept_socket(void)
{
    int fds_num;
    int counter;
    int socket;
    size_t i;
    std::vector<std::string> clients;

    i = sizeof(serverAddress);
    memset(fds, 0, MAX_CLIENTS * sizeof(struct pollfd));
    fds[0].fd = this->socket_fd;
    fds[0].events = POLLIN;
    fds_num = 1;
    counter = 0;

    while(1)
    {
        if(poll(fds, fds_num, -1) == -1)
        {
            std::cout << "Failed to poll" << std::endl;
            close(this->socket_fd);
            return;
        }
        for(int j = 0; j < fds_num; j++)
        {
            if(fds[j].revents & POLLIN)
            {
                if(fds[j].fd == this->socket_fd)
                {
                    if((socket = accept(this->socket_fd, (struct sockaddr*)&serverAddress, (socklen_t*)&i)) == -1)
                    {
                        std::cout << "Failed to accept connection" << std::endl;
                        close(this->socket_fd);
                        return;
                    }
                    if(fds_num == MAX_CLIENTS)
                    {
                        std::cout << "Maximum number of clients reached" << std::endl;
                        close(socket);
                    }
                    fds[fds_num].fd = socket;
                    fds[fds_num].events = POLLIN;
                    fds_num++;
                    std::cout << "Client connected" << std::endl;
                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(fds[fds_num- 1].fd, buffer, 1024, 0);
                    if(bytes <= 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        close(fds[fds_num- 1].fd);
                        fds[fds_num- 1].fd = -1;
                        counter++;
                    }
                    else
                    {
                        std::cout << "Received " << bytes << " bytes" << std::endl;
                        std::cout << "from client: " << buffer << std::endl;
                        if(strncmp(buffer, "PASS ", 5) == 0)
                        {
                            std::string pass =  std::string(buffer).substr(5);
                            if (pass != password)
                                close(fds[fds_num- 1].fd);
                            std::cout << "Client pass: " << pass << pass.length() << std::endl;
                            std::cout << "pass: " << password<< password.length() << std::endl;
                        }
                    }
                }
                else
                {
                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(fds[j].fd, buffer, 1024, 0);
                    if(bytes <= 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        close(fds[j].fd);
                        fds[j].fd = -1;
                        counter++;
                    }
                    else
                    {
                        std::cout << "Received " << bytes << " bytes" << std::endl;
                        std::cout << "from client: " << buffer << std::endl;


                        // std::string message = "Hello from server";
                        // if(send(fds[j].fd, message.c_str(), message.size(), 0) == -1)
                        // {
                        //     std::cout << "Failed to send message" << std::endl;
                        //     close(this->socket_fd);
                        //     return;
                        // }
                        
                        // if(counter == 0)
                        // {
                        //     clients.push_back(buffer);
                        //     counter++;
                        // }
                        // else
                        // {
                        //     for(size_t k = 0; k < clients.size(); k++)
                        //     {
                        //         if(clients[k] == buffer)
                        //         {
                        //             std::cout << "Client already exists" << std::endl;
                        //             break;
                        //         }
                        //         else
                        //         {
                        //             clients.push_back(buffer);
                        //             std::cout << "Client added" << std::endl;
                        //             break;
                        //         }
                        //     }
                        // }
                    }
                }
            }
        }
    }
}