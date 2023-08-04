#include "headerfile.hpp"

int verify_to_kick(std::string nick, std::vector<Client> users)
{
    for(size_t i = 0; i < users.size(); i++)
    {
        if(users[i].get_nickname() == nick)
            return(users[i].get_socket_client());
    }
    return -1;
}


std::string Server::kick(std::string input, Client &client)
{
   std::string response = "";
   std::string channel_name;
    std::string nickname;
    std::vector<std::string> tokens;
    std::string reason;
    int to_kick;

    if(!client.get_isRegistred())
    {
        response = ":localhost (451) ERR_NOTREGISTERED :You have not registered\r\n";
        return (response);
    }
    // Split the input buffer into tokens using ' ' (space) as the delimiter
    if(!input.empty())
    {
        size_t start = 0;
        size_t end = 0;
        while ((end = input.find(' ', start)) != std::string::npos) {
            tokens.push_back(input.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(input.substr(start));
        channel_name = tokens[1];
        nickname = tokens[2];
        if(tokens.size() == 4)
            reason = tokens[3];
        else
            reason = "";
        if(channelExists(this->_channels, channel_name))
        {
            if(!this->_channels[getChannel(channel_name)].is_moderator(client.get_socket_client()))
            {
                response = ":localhost (482) ERR_CHANOPRIVSNEEDED " + nickname + " :You're not channel operator\r\n";
                // return(response);
            }
            else if(isOnChannel(this->_channels[getChannel(channel_name)], client.get_socket_client()))
            {
                to_kick = verify_to_kick(nickname, this->clients);
                if(to_kick != -1)
                {
                    if(this->_channels[getChannel(channel_name)].is_member(to_kick))
                    {
                        
                        client.remove_channel(channel_name);
                        this->_channels[getChannel(channel_name)].addBan(nickname);
                        // client.set_kicked(true);
                        response = ":" + client.get_nickname() + " KICK " + channel_name + " " + nickname + " :" + reason + "\r\n";
                        this->_channels[getChannel(channel_name)].broadcast_message(response, client.get_socket_client());
                        this->_channels[getChannel(channel_name)].removeMember(to_kick);
                    }
                    else
                    {
                        response = ":localhost (401) ERR_NOSUCHNICK" + nickname +  " :No such nick/channel\r\n";
                    }
                }
            }
            else
            {
                response = ":localhost (441) ERR_USERNOTINCHANNEL " + nickname + " " + channel_name + " :They aren't on that channel\r\n";
            }
        }
        else
        {
            response = ":localhost (403) ERR_NOSUCHCHANNEL" + channel_name + " :No such channel\r\n";
        }

    }
    else
        response = ":localhost (461) ERR_NEEDMOREPARAMS :Not enough parameters\r\n";
    return(response);
}