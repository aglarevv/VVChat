#include "UserModel.hpp"
#include "User.hpp"
#include "connectionPool.hpp"
#include "db.hpp"


#include <iostream>




bool UserModel::insert(User& user){
    RandomAccountGenerator* randAccount = RandomAccountGenerator::instance(USER);
    std::string account;
    while(account.size() < 11){
        account = randAccount->generateUniqueAccount();
        std::cout << account << " :account\n";
    }


    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, account,password, state) values('%s','%s', '%s', '%s')",
     user.getName().c_str(), account.c_str(), user.getPassword().c_str(), user.getState().c_str());
     user.setAccount(account);
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();
    if(mysql != nullptr){
        if(mysql->update(sql)){
            return true;
        }
    }
     return false;
}

User UserModel::query(std::string& account){
    char sql[1024] = {0};
    sprintf(sql, "select * from user where account = '%s'",account.c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();
    if(mysql != nullptr){
        MYSQL_RES* res = mysql->query(sql);
        if(res != nullptr){
            //获取用户信息
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr){
                //设置用户信息
                User user;
                user.setName(row[1]);
                user.setAccount(row[2]);
                user.setPassword(row[3]);
                user.setState(row[4]);
                mysql_free_result(res);
                return user;
            }
        }
    }
    return User();
}

bool UserModel::updataState(User& user){
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where account = '%s'",
        user.getState().c_str(), user.getAccount().c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();
    if(mysql != nullptr){
        if(mysql->update(sql)){
            return true;
        }
    }
    return false;
}

void UserModel::resetState(){
    char sql[1024] = {"update user set state = 'offline' where state = 'online'"};
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection();
    if(mysql != nullptr){
        mysql->update(sql);
    }
}