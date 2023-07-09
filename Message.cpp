#include "Message.hpp"

Message::Message(int socket)
{
    this->socket = socket;
    this->params = std::vector<std::string>();
}
