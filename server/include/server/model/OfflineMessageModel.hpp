#pragma once

#include <string>
#include <vector>

class ConnectionPool;
class OfflineMessageModel {
    public:
        //存储离线消息
        bool insert(std::string userAccount, std::string message);
        //删除离线消息
        bool remove(std::string userAccount);
        //查询离线消息
        std::vector<std::string> query(std::string userAccount);
};