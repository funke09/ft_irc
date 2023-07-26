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

static void 	split_command(std::string buff, std::vector<std::string> &split)
{
	std::vector<std::string> tmp;
	size_t pos = buff.find(":");
	if (pos != std::string::npos){
		tmp.push_back(buff.substr(0, pos));
		tmp.push_back(buff.substr(pos + 1));
	}
	else
		tmp.push_back(buff);
	split = ft_split(tmp[0], ' ');
	if (tmp.size() > 1)
		split.push_back(tmp[1]);
}


std::string		Server::privmsg(std::string buff, Client &client)
{
	std::vector<std::string>	split;
	std::vector<std::string>	recipient;

	if(!client.get_isRegistred())
		return (":localhost 451 * PRIVMSG :You must finish connecting with nickname first.\r\n");
	split_command(buff, split);
	if (split.size() == 1)
		return (client.get_socket_client(), ":" + client.get_nickname() + " 461 PRIVMSG :Not enough parameters\r\n");
	else if (split.size() == 2)
		return (client.get_socket_client(), ":" + client.get_nickname() + " 412 PRIVMSG :No text to send\r\n");
	else
	{
		recipient = ft_split(split[1], ','); //split recipient
		std::vector<std::string>::iterator it = recipient.begin();
		for(; it != recipient.end(); it++)
		{
			// if (this->_channels.find(*it) != this->_channels.end() && client.check_member(*it))
			// 	this->_channels[*it].broadcast_message(":" + client.get_nick() + " PRIVMSG " + *it + " :" + split[2] + "\r\n", client.get_fd());
			if (findClientSocket(this->clients, *it))
			{
				std::string tmp = ":" + client.get_nickname() + " PRIVMSG " + *it + " :" + split[2] + "\r\n";
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