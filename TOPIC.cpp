#include "headerfile.hpp"


std::string Server::parss_topic(std::string buffer , Client &client) {
        std::vector<std::string> tokens;
        std::string response;
      
        std::string channel_name;

        // Split the input buffer into tokens using ' ' (space) as the delimiter
        if (!buffer.empty() && this->_channels[getChannel(channel_name)].getTopicMode()) {
            size_t start = 0;
            size_t end = 0;
            while ((end = buffer.find(' ', start)) != std::string::npos) {
                tokens.push_back(buffer.substr(start, end - start));
                start = end + 1;
            }
            tokens.push_back(buffer.substr(start));
            if (tokens.size() >= 2 && tokens[0] == "TOPIC" && tokens[1][0] == '#' && tokens[1].size() > 1) {
                    channel_name = tokens[1].substr(1); // Remove '#' from the channel name
                // set topic for channel
                if (tokens.size() >= 3 && tokens[2][0] == ':' && tokens[2][1] != '\n') {
                    // Topic is set
                    std::string topic = tokens[2].substr(1); // Remove ':' from the topic

                    if (channelExists(this->_channels, channel_name)) {

                        if(isOnChannel(client.get_channels(), channel_name))
                        {
                            // _topic = topic;
                            this->_channels[getChannel(channel_name)].setTopic(topic);
                            response = "Topic set for channel " + channel_name + " :" + topic + "\r\n";
                        }
                        else
                        {
                            response = ":localhost (442) ERR_NOTONCHANNEL: " + channel_name + " :You're not on that channel\r\n";
                        }
                    } 
                    else {
                        response = ":localhost (403) ERR_NOSUCHCHANNEL: " + channel_name + " :No such channel\r\n";
                    }
                }
// unset topic for channel 
                else if (tokens[2][0] == ':'){
                    // Topic is unset
                    // _name = channel_name;
            
                    this->_channels[getChannel(channel_name)].setTopic("");
                    response = "Topic unset for channel " + channel_name + "\r\n";
                }
                // Viewing a set topic
            //     else 
            //     {
                    if(this->_channels[getChannel(channel_name)].getTopic().empty())
                    {
                        response = ":localhost (331) RPL_NOTOPIC " + channel_name + " :No topic is set\r\n";
                    }
                    else
                    {
                        response = ":localhost (332) RPL_TOPIC " + channel_name + " :" + this->_channels[getChannel(channel_name)].getTopic() + "\r\n";
                    }
            //     }
            } 
            else {
                // Invalid input, handle the error (e.g., print an error message)
                response = ":localhost (461) ERR_NEEDMOREPARAMS :Not enough parameters\r\n";
            }
        }
        else
            response = ":localhost (482) ERR_CHANOPRIVSNEEDED " + channel_name + " :You're not channel operator\r\n";
        return (response);
 }