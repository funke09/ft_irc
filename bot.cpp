#include "headerfile.hpp"

std::string getCurrentTime() {
        time_t currentTime;
        time(&currentTime);
        return std::string(ctime(&currentTime));
    }

std::string Server::bot(Client &client)
{
    std::string messageText = "HELLO FROM BOT:  '" + client.get_nickname() + "' the current time is " + getCurrentTime();
    std::string response;
    response = ":localhost PRIVMSG " + client.get_nickname() + messageText + "\r\n";    
    return (response);

}