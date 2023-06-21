#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Define the maximum number of clients that the server can handle
#define MAX_CLIENTS 100

// Structure to hold client information
struct Client {
    int socket;
    std::string nickname;
    std::string username;
};
std::vector<Client> clients;

// Structure to hold channel information
struct Channel {
    std::string name;
};

void handleConnection(int newSocket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive initial data from the client (e.g., authentication)
    int bytesRead = recv(newSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == -1) {
        perror("Failed to receive data from client");
        close(newSocket);
        return;
    }

    // Process the received data (e.g., perform authentication)
    // Add your logic here based on the received data

    // Create a new Client object
    Client newClient;
    newClient.socket = newSocket;
    // Set other initial client information as needed

    // Add the new client to the clients vector
    clients.push_back(newClient);

    // Send a welcome message or perform other initial communication with the client
    const char* welcomeMessage = "Welcome to the IRC server!";
    if (send(newSocket, welcomeMessage, strlen(welcomeMessage), 0) == -1) {
        perror("Failed to send data to client");
        close(newSocket);
        return;
    }

    // Handle communication with the client
    while (true) {
        memset(buffer, 0, sizeof(buffer));

        // Receive data from the client
        int bytesRead = recv(newSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1) {
            perror("Failed to receive data from client");
            break;
        } else if (bytesRead == 0) {
            // Client has disconnected
            break;
        }

        // Process the received data (e.g., parse IRC commands)
        // Add your logic here based on the received data

        // Send a response back to the client
        const char* response = "Received your message!";
        if (send(newSocket, response, strlen(response), 0) == -1) {
            perror("Failed to send data to client");
            break;
        }
    }

    // Remove the client from the clients vector and close the socket
    std::vector<Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if (it->socket == newSocket) {
            clients.erase(it);
            break;
        }
    }
    close(newSocket);
}


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    // Extract the port and password from the command line arguments
    int port = atoi(argv[1]);
    std::string password = argv[2];

    int serverSocket;
    std::vector<Channel> channels;

    // Create the server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Set socket options to make it reusable
    int reuse = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == -1) {
        perror("Failed to set socket options");
        close(serverSocket);
        return 1;
    }

    // Bind the socket to a specific IP address and port
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Failed to bind socket");
        close(serverSocket);
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Failed to listen for connections");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server started and listening on port " << port << std::endl;

    // Main server loop
    /* This is the main server loop that listens for incoming connections and handles client activity. */
    while (true) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);

        int maxSocket = serverSocket;

        // Add client sockets to the set
        for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
            FD_SET(it->socket, &readSet);
            if (it->socket > maxSocket) {
                maxSocket = it->socket;
            }
        }

        // Wait for activity on any socket
        int activity = select(maxSocket + 1, &readSet, NULL, NULL, NULL);
        if (activity == -1) {
            perror("Error in select");
            close(serverSocket);
            return 1;
        }

        // Check if there is a new incoming connection
        if (FD_ISSET(serverSocket, &readSet)) {
            int newSocket;
            if ((newSocket = accept(serverSocket, NULL, NULL)) == -1) {
                perror("Failed to accept incoming connection");
                close(serverSocket);
                return 1;
            }

            // Handle the new connection
            handleConnection(newSocket);
        }

        // Handle client activity
        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ) {
            int clientSocket = it->socket;
            if (FD_ISSET(clientSocket, &readSet)) {
                // Handle incoming message from the client
                // implement the logic here based on the received data

                // If the client has disconnected, remove it from the clients vector
                // and close the socket
                // clients.erase(it++);
                // close(clientSocket);
            } else {
                ++it;
            }
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}