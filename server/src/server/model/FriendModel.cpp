#include "FriendModel.hpp"
#include "connectionPool.hpp"
#include "db.hpp"

#include <iostream>


bool FriendModel::insert(std::string userAccount, std::string friendAccount){
    char sql[1024] = {0};
    sprintf(sql, "insert into friend(userAccount, friendAccount) \
     select '%s', '%s' from DUAL where exists(select account from user where account = '%s' and account != '%s')",
     userAccount.c_str(), friendAccount.c_str(), friendAccount.c_str(), userAccount.c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();    
    if(mysql != nullptr){
       //等于调用mysql.update(sql)
        MYSQL_RES* res = mysql->query(sql);

        if(mysql_affected_rows(mysql->getConnection()) > 0){
            mysql_free_result(res);
            return true;
        }
        //插入失败
        mysql_free_result(res);
        return false;
    }
}

std::vector<User> FriendModel::query(std::string userAccount){
    char sql[1024] = {0};
    sprintf(sql, "select a.account, a.name, a.state from user a inner \
     join friend b on b.userAccount = a.account where b.friendAccount = '%s'", userAccount.c_str());
    std::vector<User> vec;
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();   
    if(mysql != nullptr){
        //获取sql语句执行结果
        MYSQL_RES* res = mysql->query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            //获取一行数据
            while ((row = mysql_fetch_row(res)) != nullptr){
                User user;
                user.setAccount(row[0]);
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }            
        }
        //释放资源
        mysql_free_result(res);
    }
    return vec;
}