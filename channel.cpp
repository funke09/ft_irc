#include "channel.hpp"

channel::channel(std::string name) : name(name)
{
}

channel::~channel()
{
}

channel::channel()
{
   
}

// static std::vector<std::string> ft_split(const std::string& str, char delimiter)
// {
//     std::vector<std::string> tokens;
//     std::stringstream ss(str);
//     std::string token;

//     while (std::getline(ss, token, delimiter))
//     {
//         tokens.push_back(token);
//     }

//     return tokens;
// }

// C  ->  TOPIC #v3 :This is a cool channel!!
// void channel::parss_channel(std::string buffer)
// {
//    std::vector<std::string> tokens;
//    std::cout << "buffer: " << buffer << std::endl;

//    tokens = ft_split(buffer, ' ');
//    if(tokens[0] == "TOPIC" && tokens.size() == 3 && tokens[1][0] == '#' && tokens[2][0] == ':')
//    {
//       this->name = tokens[1].substr(1, tokens[1].size() - 1);
//       std::cout << "name: " << this->name << std::endl;
//    }
//    for(size_t i = 0; i < tokens.size(); i++)
//    {
//       std::cout << "tokens[" << i << "]: " << tokens[i] << std::endl;
//    }
// }

std::string channel::parss_topic(std::string buffer)
{
  std::vector<std::string> tokens;
  std::string response;
      // if(buffer.size() <= 1 && buffer[buffer.size() - 1] == '\n')
      //   std::cout << "buffer: " << buffer << std::endl;

        // Split the input buffer into tokens using ' ' (space) as the delimiter
        if(!buffer.empty())
        {
            size_t start = 0;
            size_t end = 0;
            while ((end = buffer.find(' ', start)) != std::string::npos) {
                  tokens.push_back(buffer.substr(start, end - start));
                  start = end + 1;
            }
            tokens.push_back(buffer.substr(start));

            if (tokens.size() >= 2 && tokens[0] == "TOPIC" && tokens[1][0] == '#') {
                  if (tokens[2][0] == ':') {
                     // Topic is set
                     this->name = tokens[1].substr(1); // Remove '#' from the channel name
                     this->topic = tokens[2].substr(1); // Remove ':' from the topic
                     response = "Topic set for channel " + this->name + ": " + this->topic + "\n";
                  } else {
                     // Topic is unset
                     this->name = tokens[1].substr(1); // Remove '#' from the channel name
                     this->topic = ""; // Empty string indicates no topic set
                     response = "Topic unset for channel " + this->name + "\n";
                  }
            } else {
                  // Invalid input, handle the error (e.g., print an error message)
                  response = "Invalid TOPIC format.\n";
            }
        }
         return response;
}