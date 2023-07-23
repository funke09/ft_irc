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

class Server;
class Client;

class Channel {    
    private:
        std::string _name;
        std::string _pass;
        std::string _topic;
        std::string _mode;
        std::string _creation_time;
        int            _limit;
        bool        _is_private;
        bool        _inviteMode;
        bool        _topicMode;
        bool        _limitMode;

        std::vector<int>            _members;
        std::vector<int>            _moderators;
        std::vector<std::string>    _bans_list;
        std::vector<std::string>    _invited_list;
        std::vector<std::string>    _operators;

    public:
        Channel(const std::string& name, const std::string& pass, const std::string& topic);
        Channel(const std::string& name, Client *cl);

        const std::string& getName() const;
        const std::string& getPass() const;
        const std::string& getTopic() const;
        const std::string& getMode() const;
        const std::string& getCreationTime() const;
        const std::vector<int>& getMembers() const;
        const std::vector<int>& getModerators() const;
        const std::vector<std::string>& getBansList() const;
        const std::vector<std::string>& getInvitedList() const;

        void setName(const std::string& name);
        void setPass(const std::string& pass);
        void setTopic(const std::string& topic);
        void setMembers(const std::vector<int>& members);
        void setModerators(const std::vector<int>& moderators);
        void setBansList(const std::vector<std::string>& bansList);
        void setInvitedList(const std::vector<std::string>& invitedList);

        void addMember(int memberId);
        
        void removeMember(int memberId);
        void addModerator(int moderatorId);
        void removeModerator(int moderatorId);
        void addBan(const std::string& ban);
        void removeBan(const std::string& ban);
        void inviteUser(const std::string& user);
        void removeInvitation(const std::string& user);

        void setOperator(Client *client);
        bool validateJoin(Client *client, std::string key);
        bool isBanned(Client *client) const;
        //void changeMode(Client* client, const std::string& modeChanges);
};

bool isChannelInVector(const std::vector<Channel>& channels, const std::string& targetChannel);
bool operator==(const Channel& lhs, const Channel& rhs);


#endif
