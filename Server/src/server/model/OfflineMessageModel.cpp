#include "OfflineMessageModel.hpp"
#include "connectionPool.hpp"
#include "db.hpp"


// 存储离线消息
bool OfflineMessageModel::insert(std::string userAccount, std::string message){
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage(userAccount, message) values('%s', '%s')",
     userAccount.c_str(), message.c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection(); 
    if (mysql != nullptr){
        mysql->update(sql);
        return true;
    }
    return false;
}
// 删除离线消息
bool OfflineMessageModel::remove(const std::string userAccount){
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userAccount='%s'", userAccount.c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();
    if (mysql != nullptr){
        mysql->update(sql);
        return true;
    }
    return false;
}
// 查询离线消息
std::vector<std::string> OfflineMessageModel::query(std::string userAccount){
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userAccount = '%s'",userAccount.c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();
    std::vector<std::string> vec;
    if(mysql != nullptr){
        MYSQL_RES* res = mysql->query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr){
                vec.push_back(row[0]);
            }
        }
        mysql_free_result(res);  
    }
    return vec;
}
