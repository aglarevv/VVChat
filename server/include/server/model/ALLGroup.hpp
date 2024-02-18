#pragma once

#include "GroupUser.hpp"

#include <vector>
#include <string>


class AllGroup {
    public:
        AllGroup(std::string groupAccount = "", std::string groupName = "", std::string groupDesc = ""){
            this->groupAccount = groupAccount;
            this->groupName = groupName;
            this->groupDesc = groupDesc;
        }
        void setGroupAccount(std::string groupAccount) { this->groupAccount = groupAccount; }
        void setGroupName(std::string groupName) { this->groupName = groupName; }
        void setGroupDesc(std::string groupDesc) { this->groupDesc = groupDesc; }

        std::string getGroupAccount() { return this->groupAccount; }
        std::string getGroupName() { return this->groupName; }
        std::string getGroupDesc() { return this->groupDesc; }
        //获取群组成员
        std::vector<GroupUser>& getUsers() { return this->users;}

    private:
        std::string groupAccount;
        std::string groupName;
        std::string groupDesc;
        //群组成员
        std::vector<GroupUser> users;
};