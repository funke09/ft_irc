#include "headerfile.hpp"

static int check_inVect(std::vector<int> vec, int target)
{
	for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	    {
	        if (*it == target)
	            return true;
	    }
	    return false;
}

std::string 	Server::mode_response(std::vector<std::string> split, Client &client)
{
	std::string response;
	int			flg;

	if(!client.get_isRegistred())
		return (":localhost 451 * MODE :You must finish connecting with nickname first.\r\n");
	if(split.size() < 2)
		return (":localhost 461 " + client.get_nickname() + " " + split[0] + " :Not enough parameters\r\n");
	if(!isChannelInVector(this->_channels, split[1]))
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
			if (x == 't' && !channel.getTopicMode())
			{
				mode += "i";
				response += "i";
				channel.set_topicMode(true);
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
		}
	}
	if(flg)
		return (response + "\r\n");
	return "";
}