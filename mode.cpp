#include "headerfile.hpp"

static bool check_inVect(std::vector<int> vec, int target)
{
	for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	    {
	        if (*it == target)
	            return true;
	    }
	    return false;
}

static int find_client(Channel channel, std::vector<Client> clients, std::string client_name)
{
	
	for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	    {
	        if (it->get_nickname() == client_name)
			{
				if (check_inVect(channel.getMembers(), it->get_socket_client()))
					return it->get_socket_client();
			}
	    }
	    return 0;
}

static bool isStringAllDigits(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

std::string 	Server::mode_response(std::vector<std::string> split, Client &client)
{
	std::string response;
	int			flg;

	if(!client.get_isRegistred())
		return (":localhost 451 * MODE :You must finish connecting with nickname first.\r\n");
	if(split.size() < 2)
		return (":localhost 461 " + client.get_nickname() + " " + split[0] + " :Not enough parameters\r\n");
	if(!ChanmodeExists(this->_channels, split[1]))
		return (":localhost 403 " + client.get_nickname() +  " " + split[1] + " :No such channel\r\n");
	Channel &channel = this->_channels[isChannelInVector(this->_channels, split[1])];
	if(split.size() == 2 && check_inVect(channel.getMembers(), client.get_socket_client()))
		return (":localhost 324 " + client.get_nickname() + " " + split[1] + " +" + channel.getMode() + 
				"\n:localhost 329 " + client.get_nickname() + " " + split[1] + " " + 
				channel.getCreationTime() + "\r\n");
	if(!check_inVect(channel.getModerators(), client.get_socket_client()))
		return (":localhost 482 " + client.get_nickname() + " " + split[1] + " :You're not channel operator\r\n");
	std::string mode = channel.getMode();
	flg = 0;
	response = ":" + client.get_nickname() + "!~" + client.get_username() + "@localhost MODE " + channel.getName();
	if (split[2][0] == '+')
	{
		response += "+";
		for (int i = 0; split[2][i]; i++)
		{
			char x = split[2][i];
			if (x == 'i' && !channel.getInvitedMode())
			{
				mode += "i";
				response += "i";
				channel.set_invitedMode(true);
				flg++;
			}
			if (x == 't' && !client.get_topicMode())
			{
				mode += "t";
				response += "t";
				client.set_topicMode(true);
				flg++;
			}
			if(x == 'k' && !channel.getPrivate() && split.size() >= 4)
			{
				std::string key = split[3];
				channel.set_private(true);
				channel.set_pass(key);
				mode += "k " + key + " ";
				response += "k " + key + " ";
				flg++;
			}
			if(x == 'l' && !channel.get_limitMode() && split.size() >= 4 && isStringAllDigits(split[3]))
			{
				int limit = atoi(split[3].c_str());
				if((size_t)limit < channel.getMembers().size())
					limit = (int)channel.getMembers().size();
				channel.set_limitMode(true);
				channel.setLimit(limit);
				mode += "l " + std::to_string(limit) + " ";
				response += "l " + std::to_string(limit) + " ";
				flg++;
			}
			if(x == 'o' && channel.is_moderator(client.get_socket_client()) && split.size() >= 4 && find_client(channel, clients, split[3]))
			{
				std::string user = split[3];
				channel.addModerator(find_client(channel, clients, user));
				mode += "o " + user + " ";
				response += "o " + user + " ";
				flg++;
			}
		}
	}
	else if(split[2][0] == '-')
	{
		response += "-";
		for (int i = 0; split[2][i]; i++)
		{
			char x = split[2][i];
			if (x == 'i' && channel.getInvitedMode())
			{
				mode.erase(mode.find('i'), 1);
				response += "i";
				channel.set_invitedMode(false);
				flg++;
			}
			if (x == 't' && client.get_topicMode())
			{
				mode.erase(mode.find('t'), 1);
				response += "t";
				client.set_topicMode(false);
				flg++;
			}
			if(x == 'k' && channel.getPrivate() && split.size() >= 4)
			{
				std::string oldkey = channel.getPass();
				std::string key = split[3];
				channel.set_private(false);
				channel.set_pass("");
				mode.erase(mode.find("k " + oldkey + " "), oldkey.length( ) + 3);
				response += "k " + key;
				flg++;
			}
			if(x == 'l' && channel.get_limitMode())
			{
				std::string limit = std::to_string(channel.get_limit());
				channel.set_limitMode(false);
				channel.setLimit(MAXCHAN);
				mode.erase(mode.find("l " + limit + " "), limit.length( ) + 3);
				response += "l" ;
				flg++;
			}
			if(x == 'o' && channel.is_moderator(client.get_socket_client()) && split.size() >= 4 && find_client(channel, clients, split[3]))
			{
				std::string user = split[3];
				channel.removeModerator(find_client(channel, clients, split[3]));
				response += "o " + user + " ";
				flg++;
			}
			else
				std::cout << "Error :Flag "<< x << " not supported\r\n" ;
			
		}
	}
	
	channel.setMode(mode);
	if(flg)
		return (response + "\r\n");
	return "";
}