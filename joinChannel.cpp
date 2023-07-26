#include "headerfile.hpp"

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

std::string Server::ft_time(void)
{
	time_t curr_time;
	curr_time = time(NULL);
	return std::to_string(curr_time);
}

bool Server::isOnChannel(Client &client)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].get_nickname() == client.get_nickname())
        {
            return true; 
        }
    }
    return false; 
}

Channel Server::getChannel(std::string channelName)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == channelName)
            return _channels[i];
    }
    return Channel();
}

bool Server::channelExists(const std::string& targetName) const
{
    std::vector<Channel>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == targetName)
            return true;
    }
    return false;
}

std::string Server::joinChannel(std::vector<std::string> pars, Client& client)
{
    Channel chan;

    if(pars.size() == 1 || (pars.size() == 2 && pars[1] == "#"))
		return(":localhost 461 " + client.get_nickname() + " " + pars[0] + " :Not enough parameters\r\n");
	else if(pars.size() >= 2)
	{
		std::vector<std::string> names;
		std::vector<std::string> key;
		names = ft_split(pars[1], ',');
		if(pars.size() > 2)
			key = ft_split(pars[2], ',');
		for(size_t i = 0; i < names.size(); i++)
		{
			if(names[i][0] != '#')
				return ":localhost 403 " + client.get_nickname() + " " + names[i] + " :No such channel\r\n";
			else if(isOnChannel(client))
				return ":localhost 443 " + client.get_nickname() + " " + names[i] + ":is already on channel\r\n";
			else
			{
				if(!channelExists(names[i]))
				{
                    client.set_channel(names[i]);
					if(key.size() > i)
						chan = Channel(names[i], key[i]);
					else
						chan = Channel(names[i], "");

					chan.addMember(client.get_socket_client());
					chan.addModerator(client.get_socket_client());
					chan.set_creationTime(ft_time());
					_channels.push_back(chan);

					return (client.get_socket_client(), ":" + client.get_nickname() + "!" + client.get_username() + " JOIN :" + names[i] + "\r\n");
				}
                else
                {
                    chan = getChannel(names[i]);
					if(chan.get_limitMode() && chan.get_limit() <= static_cast<int>(chan.getMembers().size()))
						return (client.get_socket_client(), ":localhost 471 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+l)\r\n");
					else if(chan.getInvitedMode())
					{
						if(chan.isInvited(client.get_nickname()))
						{
							client.set_channel(names[i]);

							chan.addMember(client.get_socket_client());
							return (client.get_socket_client(), ":" + client.get_nickname() + "!~" + client.get_username() + " JOIN :" + names[i] + "\r\n");
						}
						else
							return (client.get_socket_client(), ":localhost 473 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+i)\r\n");
					}
					else
					{
						if(key.size() > i)
						{
							if(chan.getPass() == key[i])
							{
								client.set_channel(names[i]);
								chan.addMember(client.get_socket_client());
								return (client.get_socket_client(), ":" + client.get_nickname() + "!~" + client.get_username() + " JOIN :" + names[i] + "\r\n");
							}
							else
								return (client.get_socket_client(), ":localhost 475 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+k)\r\n");
						}
						else
						{
							if(chan.getPass() == "")
							{
								client.set_channel(names[i]);
								chan.addMember(client.get_socket_client());
								return (client.get_socket_client(), ":" + client.get_nickname() + "!~" + client.get_username() + " JOIN :" + names[i] + "\r\n");
							}
							else
								return (client.get_socket_client(), ":localhost 475 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+k)\r\n");
						}
		            }
                }
            }
        }
    }
    return "";
}