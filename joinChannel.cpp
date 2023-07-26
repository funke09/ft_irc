#include "headerfile.hpp"

std::string get_adderss()
{
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    struct hostent *host = gethostbyname(hostname);
    if (host)
        return std::string(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    return "localhost";
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

std::string Server::ft_time(void)
{
	time_t curr_time;
	curr_time = time(NULL);
	return std::to_string(curr_time);
}



int Server::getChannel(std::string channelName)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == channelName)
            return i;
    }
    return 0;
}

bool channelExists(std::vector<Channel> channels, std::string& targetName)
{
    std::vector<Channel>::const_iterator it;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == targetName)
            return true;
    }
    return false;
}

bool isOnChannel(const std::vector<std::string>& channels, const std::string& input)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i] == input)
        {

            return true; 
        }
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
		std::string response;
		names = ft_split(pars[1], ',');
		if(pars.size() > 2)
			key = ft_split(pars[2], ',');
		
		for(size_t i = 0; i < names.size(); i++)
		{
			if(names[i][0] != '#')
				return ":localhost 403 " + client.get_nickname() + " " + names[i] + " :No such channel\r\n";
			else if(isOnChannel(client.get_channels(), pars[1]))
				return ":localhost 443 " + client.get_username() + " " + names[i] + ":is already on channel\r\n";
			else
			{
				
				if(!channelExists(this->_channels , names[i]))
				{
					
                    client.set_channel(names[i]);
					if(key.size() > i)
						chan = Channel(names[i], key[i]);
					else
						chan = Channel(names[i], "");
					
					std::cout << client.get_nickname()<<std::endl;
					chan.addMember(client.get_socket_client());
					chan.addModerator(client.get_socket_client());
					chan.set_creationTime(ft_time());
					_channels.push_back(chan);
					response = ":" + client.get_nickname() + "!" + client.get_username() + "@" + get_adderss() + " JOIN :" + names[i] + "\r\n";
					send(client.get_socket_client(), response.c_str(), response.size(), 0);
				}
                else
                {
					if(_channels[getChannel(names[i])].get_limitMode() && _channels[getChannel(names[i])].get_limit() <= static_cast<int>(_channels[getChannel(names[i])].getMembers().size()))
					{
						response = ":localhost 471 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+l)\r\n";
						send(client.get_socket_client(), response.c_str(), response.length(), 0);

					}
					else if(_channels[getChannel(names[i])].getInvitedMode())
					{
						if(_channels[getChannel(names[i])].isInvited(client.get_nickname()))
						{
							client.set_channel(names[i]);
							_channels[getChannel(names[i])].addMember(client.get_socket_client());
							response = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + get_adderss() + " JOIN :" + names[i] + "\r\n";
							_channels[getChannel(names[i])].broadcast_message(response, 0);
							// send(client.get_socket_client(), response.c_str(), response.length(), 0);
						}
						else
						{
							response = ":localhost 473 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+i)\r\n";
							send(client.get_socket_client(), response.c_str(), response.length(), 0);
						}
					}
					else
					{
						if(key.size() > i)
						{
							if(_channels[getChannel(names[i])].getPass() == key[i])
							{
								client.set_channel(names[i]);
								_channels[getChannel(names[i])].addMember(client.get_socket_client());
								response = ":" + client.get_nickname() + "!~" + client.get_username() + " JOIN :" + names[i] + "\r\n";
								_channels[getChannel(names[i])].broadcast_message(response, 0);
							}
							else
							{
								response = ":localhost 475 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+k)\r\n";
								send(client.get_socket_client(), response.c_str(), response.length(), 0);
							}
						}
						else
						{
							if(_channels[getChannel(names[i])].getPass() == "")
							{
								client.set_channel(names[i]);
								_channels[getChannel(names[i])].addMember(client.get_socket_client());
								response = ":" + client.get_nickname() + "!~" + client.get_username() + " JOIN :" + names[i] + "\r\n";
								_channels[getChannel(names[i])].broadcast_message(response, 0);
							}
							else
							{
								response = ":localhost 475 " + client.get_nickname() + " " + names[i] + " :Cannot join channel (+k)\r\n";
								send(client.get_socket_client(), response.c_str(), response.length(), 0);
							}
						}
		            }
                }
            }
        }
    }
    return "";
}