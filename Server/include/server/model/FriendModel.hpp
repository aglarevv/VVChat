#pragma once

#include "User.hpp"

#include <string>
#include <vector>

class ConnectionPool;
class FriendModel {
    public:
        //添加好友
        bool insert(std::string userAccount, std::string friendid);
        //查询好友
        std::vector<User> query(std::string username);

};