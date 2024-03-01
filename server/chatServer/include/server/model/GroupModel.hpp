#pragma once

#include "other/randomAccountGenerator.hpp"

#include <string>
#include <vector>

class AllGroup;
class ConnectionPool;
class GroupModel {
    public:
        //创建群组 
        bool create(AllGroup& allGroup);
        //添加群员
        bool add(std::string groupAccount, std::string userAccount, std::string role);
        //查询用户所在群组信息
        std::vector<AllGroup> queryGroups(std::string userAccount);
        //查询除本用户外群员列表
        std::vector<std::string> queryUsers(std::string userAccount, std::string groupAccount);

};