#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

class channel
{
    public:
    channel();
    ~channel();

    private:
    std::string name;
    std::vector<std::string> users;
    
};

#endif
