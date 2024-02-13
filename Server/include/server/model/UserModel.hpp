#pragma once

#include "other/randomAccountGenerator.hpp"

#include <string>

class User;
class UserModel {
    public:
        //插入用户
        bool insert(User& user);
        //查询
        User query(std::string& account);
        //跟新在线状态
        bool updataState(User& user);
        //重置所有用户状态
        void resetState();
};