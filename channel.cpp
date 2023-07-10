#include "channel.hpp"

Channel::Channel(const std::string& name, const std::string& pass, const std::string& topic) {
    _name = name;
    _pass = pass;
    _topic = topic;
    _mode = "";
    _creation_time = "";
    _limit = 0;
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
