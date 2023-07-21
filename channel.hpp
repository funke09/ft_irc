#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include "Message.hpp"
#include "client.hpp"
#include "headerfile.hpp"

class channel
{
    public:
    channel(std::string name);
    channel();
    ~channel();
    std::string parss_topic(std::string buffer);

    private:
    std::string name;
    std::string topic;
    std::vector<std::string> users;
    
};

#endif
