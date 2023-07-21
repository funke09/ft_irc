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

std::string Server::privmsg(std::string buff, Client &client)
{
    std::vector<std::string> split;
    std::vector<std::string> users;

    split_command(buff, split);
    if (split.size() == 1)
        return ( "error message not enough params");
    else if(split.size() == 2)
        return ("error no text message sended");
    // else
    // {
    //     users = ft_split(split[1], ',');
    //     std::vector<std::string>::iterator  it;
    //     it = users.begin();
    //     for(; it != users.end();it++)
    //     {
    //         if (this->)
    //     }

    // }
}