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

std::string Server::parss_topic(std::string buffer, Client &client) {
	std::vector<std::string> split = ft_split(buffer, ' ');
	std::string response;
	std::string channel_name;

	if (!client.get_isRegistred()) {
		response = ":localhost 451 ERR_NOTREGISTERED :You have not registered\r\n";
		return response;
	}

	if (split.size() < 2 || (split.size() == 2 && split[1][0] != '#')) {
		response = ":localhost 461 ERR_NEEDMOREPARAMS " + split[0] + " :Not enough parameters\r\n";
		return response;
	}

	if (!channelExists(this->_channels, split[1]))
	{
		response = ":localhost 403 ERR_NOSUCHCHANNEL " + split[1] + " :No such channel\r\n";
		return response;
	}

	Channel &channel = this->_channels[getChannel(split[1])];
	if(!isOnChannel(channel, client.get_socket_client()))
	{
		response = ":localhost 442 ERR_NOTONCHANNEL " + split[1] + " :You're not on that channel\r\n";
		return response;
	}

	if ((!buffer.empty() && channel.getTopicMode()) || (!buffer.empty() && channel.is_moderator(client.get_socket_client()))) {
		// Find the first occurrence of ':'
		size_t topicStart = buffer.find(':');
		std::string topic;
		channel_name = split[1].substr(1); // Remove '#' from the channel name
		if (topicStart != std::string::npos) {
			// Extract the topic from the buffer
			if (buffer[topicStart + 1] == ':')
		   		topic = buffer.substr(topicStart + 2);
			else
		   		topic = buffer.substr(topicStart + 1);
		   	if(topic.empty())
		   		topic = "";
			channel.setTopic(topic);
			response = ":localhost 332 RPL_TOPIC " + channel_name + " :" + topic + "\r\n";
		} 
		if(topicStart == std::string::npos)
		{
			if(channel.getTopic().empty())
				response = ":localhost 331 RPL_NOTOPIC " + channel_name + " :No topic is set\r\n";
			else
				response = ":localhost 332 RPL_TOPIC " + channel_name + " :" + channel.getTopic() + "\r\n";
		}
	} 
	else 
	{
		response = ":localhost (482) ERR_CHANOPRIVSNEEDED " + channel_name + " :You're not channel operator\r\n";
	}

	return response;
}