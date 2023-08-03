#include "channel.hpp"

Channel::Channel()
{
    _name = "";
    _pass = "";
    _topic = "";
    _mode = "";
    _creation_time = "";
    _limit = 100;
    _is_private = false;
    _inviteMode = false;
    _topicMode = false;
    _limitMode = false;
}

Channel::Channel(std::string name, std::string pass)
{
    _name = name;
    _pass = pass;
    _topic = "";
    _mode = "";
    _creation_time = "";
    _limit = 100;
    _is_private = false;
    _inviteMode = false;
    _topicMode = false;
    _limitMode = false;
}

Channel::~Channel()
{
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

const std::string& Channel::getCreationTime() const
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

const bool& Channel::getInvitedMode() const
{
    return _inviteMode;
}

const bool& Channel::getTopicMode() const
{
    return _topicMode;
}

const bool& Channel::getPrivate() const
{
    return _is_private;
}

const int& Channel::get_limit() const
{
    return _limit;
}

const bool& Channel::get_limitMode() const
{
    return _limitMode;
}


bool  Channel::is_moderator(int fd) const
{
    for (size_t i = 0; i < _moderators.size(); ++i)
        {
            if (_moderators[i] == fd)
                return true;
        }
        return false;
}

bool Channel::is_member(int fd)
{
    for(size_t i = 0; i < this->_members.size(); i++)
    {
        if(_members[i] == fd)
            return true;
    }
    return false;
}


void Channel::set_creationTime(std::string time)
{
    _creation_time = time;
}

void Channel::set_limitMode(bool limitMode)
{
    _limitMode = limitMode;
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

void Channel::set_invitedMode(const bool& invitemode)
{
    _inviteMode = invitemode;
}

void Channel::set_topicMode(const bool& topicmode)
{
    _topicMode = topicmode;
}

void Channel::set_private(const bool& is_private)
{
    _is_private = is_private;
}

void Channel::set_pass(std::string pass)
{
    _pass = pass;
}

void Channel::setLimit(int limit)
{
    _limit = limit;
}

void Channel::setMode(std::string mode)
{
    _mode = mode;
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

void Channel::addInvitedList(std::string user)
{
	this->_invited_list.push_back(user);
}

bool Channel::isInvited(std::string client)
{
	for (size_t i = 0; i < this->_invited_list.size(); i++)
	{
		if (this->_invited_list[i] == client)
			return true;
	}
	return false;
}

bool Channel::isBanned(std::string nickname)
{
    if (_bans_list.size() < 1)
        return false;
    for (size_t i = 0; i < this->_bans_list.size(); ++i)
    {
        if (_bans_list[i] == nickname)
            return true;
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

int isChannelInVector(const std::vector<Channel>& channels, const std::string& targetChannel)
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
    return (-1);
}

void Channel::broadcast_message(std::string message, int fd)
{
	for (size_t i = 0; i < this->_members.size(); i++)
	{
		if(this->_members[i] != fd)
			send(this->_members[i], message.c_str(), message.size(), 0);
	}
}
