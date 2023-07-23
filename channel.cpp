#include "channel.hpp"

Channel::Channel(const std::string &name, Client *cl)
{
    (void)cl;
    _name = name;
    _topic = "";
    _mode = "";
    _limit = 100;
    _is_private = false;
}

Channel::Channel(const std::string& name, const std::string& pass, const std::string& topic) {
    _name = name;
    _pass = pass;
    _topic = topic;
    _mode = "";
    _creation_time = "";
    _limit = 100;
    _is_private = false;
    _inviteMode = false;
    _topicMode = false;
    _limitMode = false;
}

const std::string& Channel::getName() const
{
    return _name;
}

const std::string& Channel::getPass() const
{
    return _pass;
}

const std::string& Channel::getTopic() const
{
    return _topic;
}

const std::string& Channel::getMode() const
{
    return _mode;
}

std::string Channel::getCreationTime() const
{
	return	this->_creation_time;
}

const std::vector<int>& Channel::getMembers() const
{
    return _members;
}

const std::vector<int>& Channel::getModerators() const
{
    return _moderators;
}

const std::vector<std::string>& Channel::getBansList() const
{
    return _bans_list;
}

const std::vector<std::string>& Channel::getInvitedList() const
{
    return _invited_list;
}



void Channel::setName(const std::string& name)
{
    _name = name;
}

void Channel::setPass(const std::string& pass)
{
    _pass = pass;
}

void Channel::setTopic(const std::string& topic)
{
    _topic = topic;
}

void Channel::setMembers(const std::vector<int>& members)
{
    _members = members;
}

void Channel::setModerators(const std::vector<int>& moderators)
{
    _moderators = moderators;
}

void Channel::setBansList(const std::vector<std::string>& bansList)
{
    _bans_list = bansList;
}

void Channel::setInvitedList(const std::vector<std::string>& invitedList)
{
    _invited_list = invitedList;
}

void Channel::addMember(int memberId)
{
    _members.push_back(memberId);
}

void Channel::removeMember(int memberId)
{
    _members.erase(std::remove(_members.begin(), _members.end(), memberId), _members.end());
}

void Channel::addModerator(int moderatorId)
{
    _moderators.push_back(moderatorId);
}

void Channel::removeModerator(int moderatorId)
{
    _moderators.erase(std::remove(_moderators.begin(), _moderators.end(), moderatorId), _moderators.end());
}

void Channel::addBan(const std::string& ban)
{
    _bans_list.push_back(ban);
}

void Channel::removeBan(const std::string& ban)
{
    _bans_list.erase(std::remove(_bans_list.begin(), _bans_list.end(), ban), _bans_list.end());
}

void Channel::inviteUser(const std::string& user)
{
    _invited_list.push_back(user);
}

void Channel::removeInvitation(const std::string& user)
{
    _invited_list.erase(std::remove(_invited_list.begin(), _invited_list.end(), user), _invited_list.end());
}

bool Channel::isBanned(Client *client) const
{
    for (size_t i = 0; i < _bans_list.size(); i++)
    {
        if (_bans_list[i] == client->get_nickname())
        {
            return true;
        }
    }
    return false;
}

void Channel::setOperator(Client *client)
{
    _operators.push_back(client->get_nickname());
}

bool operator==(const Channel& lhs, const Channel& rhs)
{
    return lhs.getName() == rhs.getName();
}

bool isChannelInVector(const std::vector<Channel>& channels, const std::string& targetChannel)
{
    int i = 0;
    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == targetChannel)
        {
            return i;
        }
        i++;
    }
    return (0);
}

