#include "headerfile.hpp"

int findClientSocket(std::vector<Client>& clients, const std::string targetClient)
{
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->get_nickname() == targetClient)
        {
            return it->get_socket_client();
        }
    }
    return 0;
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



static void split_command(const std::string& buff, std::vector<std::string>& split)
{
    size_t pos = buff.find(":");
    if (pos != std::string::npos) {
        std::string part_before = buff.substr(0, pos);
        std::string part_after = buff.substr(pos + 1);

        // Split the part before ':' by space character
        std::vector<std::string> tmp = ft_split(part_before, ' ');

        // Append the part after ':' to the split vector
        tmp.push_back(part_after);

        split = tmp;
    } else {
        // No ':' found, split the whole string by space character
        split = ft_split(buff, ' ');
    }
}



std::string		Server::privmsg(std::string buff, Client &client)
{
	std::vector<std::string>	split;
	std::vector<std::string>	recipient;
    std::string response = "";

	if(!client.get_isRegistred())
		return (":localhost 451 * PRIVMSG :You must finish connecting with nickname first.\r\n");
    
	split_command(buff, split);
    
	if (split.size() == 1)
    {
        response = ":" + client.get_nickname() + " 411 * :No recipient given (PRIVMSG)\r\n";
		send(findClientSocket(clients, client.get_nickname()), response.c_str(), response.size(), 0);
    }
	else if (split.size() == 2)
		return (":" + client.get_nickname() + " 412 PRIVMSG :No text to send\r\n");
	else
	{
		recipient = ft_split(split[1], ',');
		std::vector<std::string>::iterator it = recipient.begin();
		for(; it != recipient.end(); it++)
		{
			if (channelExists(this->_channels, *it) && this->_channels[getChannel(*it)].is_member(client.get_socket_client()))
				this->_channels[getChannel(*it)].broadcast_message(":" + client.get_nickname() + "!~" + client.get_username() + "@" + client.get_host() + " PRIVMSG " + *it + " :" + split[2] + "\r\n", client.get_socket_client());
			else if (getclientFd(this->clients, *it) != 0)
			{
                //:nick2!~lo@5c8c-aff4-7127-3c3-1c20.230.197.ip PRIVMSG nick1 :hellothere
				std::string tmp = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + client.get_host() + " PRIVMSG " + *it + " :" + split[2] + "\r\n";
				send(findClientSocket(this->clients, *it), tmp.c_str(), tmp.size(), 0);
			}
			else
            {
                std::string tmp = "401 " + client.get_nickname() + " " + *it + " :No such nick/channel\r\n";
				send(client.get_socket_client(), tmp.c_str(), tmp.size(), 0);
            }
		}
	}
	return ("");
}