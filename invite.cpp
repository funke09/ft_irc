#include "headerfile.hpp"

bool chan_inVect(std::vector<int> vect, int value)
{
	std::vector<int>::iterator it;

	it = std::find(vect.begin(), vect.end(), value);
	if(it != vect.end())
		return true;
	return false;
}

int getclientFd(std::vector<Client> clients, std::string nickname)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].get_nickname() == nickname)
        {
            return clients[i].get_socket_client();
        }
    }
    return 0;
}

std::string	Server::invite(std::vector<std::string> input, Client &client)
{
	Channel chan;
    int clientFd;
	std::vector<Channel>::iterator it;

	if(!client.get_isRegistred())
		return (":localhost 451 * MODE :You must finish connecting with nickname first.\r\n");
	if(input.size() < 3)
		return(":localhost 461 " + client.get_nickname() + " INVITE :Not enough parameters\r\n");
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == input[2])
        {
            chan = *it;
            break;
        }
		else
			return (":localhost 403 " + client.get_nickname() + " " + input[2] + " :No such channel\r\n");
    }
	if(chan.getInvitedMode() && !chan_inVect(chan.getModerators(), client.get_socket_client()))
		return (":localhost 482 " + client.get_nickname() + " " + input[2] + " :You're not channel operator\r\n");
	if(!chan_inVect(chan.getMembers(), client.get_socket_client()))
		return (":localhost 442 " + client.get_nickname() + " " + input[2] + " :You're not on that channel\r\n");
	clientFd = getclientFd(this->clients, input[1]);
	if(chan_inVect(chan.getMembers(), clientFd))
		return (":localhost 443 " + client.get_nickname() + " " + input[1] + " " + input[2] +  " :is already on channel\r\n");
	if(clientFd)
	{
		chan.addInvitedList(input[1]);
		std::string response = ":" + client.get_nickname() + "!~" + client.get_username() + "@localhost INVITE " + input[1] + " " + input[2] + "\r\n";
		send(clientFd, response.c_str(), response.size(), 0);
	}			
	return "";
}
