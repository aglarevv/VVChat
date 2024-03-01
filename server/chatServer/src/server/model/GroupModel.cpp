#include "GroupModel.hpp"
#include "ALLGroup.hpp"
#include "connectionPool.hpp"
#include "db.hpp"

#include <vector>
#include <iostream> 
#include <cstring>


//创建群组 
bool GroupModel::create(AllGroup& allGroup){
    RandomAccountGenerator* randAccount = RandomAccountGenerator::instance(GROUP);
    std::string account = randAccount->generateUniqueAccount();

    char sql[1024] = {0};
    sprintf(sql, "insert into allgroup(groupAccount,groupName,groupDesc) values('%s','%s','%s')",
    account.c_str(), allGroup.getGroupName().c_str(), allGroup.getGroupDesc().c_str());
    allGroup.setGroupAccount(account);
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection(); 
    if(mysql != nullptr){
        if(mysql->update(sql)){
            return true;
        }
    }
    return false;
}
//添加群员
bool GroupModel::add(std::string groupAccount, std::string userAccount, std::string role){
    char sql[1024] = {0};
    //条件插入，群组存在才能插入成功
    sprintf(sql, "insert into groupuser(groupAccount,userAccount,grouprole) \
        select '%s', '%s', '%s' from DUAL where exists (select groupAccount from allgroup where groupAccount = '%s')", 
        groupAccount.c_str(), userAccount.c_str(), role.c_str(), groupAccount.c_str());
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
//查询用户所在群组信息
std::vector<AllGroup> GroupModel::queryGroups(std::string userAccount){
    char sql[1024] = {0};
    sprintf(sql, "select a.groupAccount,a.groupName,a.groupDesc from allgroup a inner \
     join groupuser b on a.groupAccount = b.groupAccount where b.userAccount = '%s'", userAccount.c_str());
    std::vector<AllGroup> vec;
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection(); 
    //获取群信息
    if(mysql != nullptr){
        MYSQL_RES* res = mysql->query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr){
                AllGroup group;
                group.setGroupAccount(row[0]);
                group.setGroupName(row[1]);
                group.setGroupDesc(row[2]);
                vec.push_back(group);
            }
            mysql_free_result(res);
        }
            
    }
    bzero(sql, sizeof(sql));
    //获取群员信息
    for(auto& group : vec){
        sprintf(sql, "select a.account,a.name,a.state,b.grouprole from user a \
         inner join groupuser b on b.userAccount = a.account where b.groupAccount = '%s'", group.getGroupAccount().c_str());
        if(mysql != nullptr){
            MYSQL_RES* res = mysql->query(sql);
            if(res != nullptr){
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != nullptr){
                    GroupUser user;
                    user.setAccount(row[0]);
                    user.setName(row[1]);
                    user.setState(row[2]);
                    user.setRole(row[3]);
                    group.getUsers().push_back(user);
                }
                mysql_free_result(res);
            }               
        }
    }
    return vec;
}
//查询除本用户外群员列表
std::vector<std::string> GroupModel::queryUsers(std::string userAccount, std::string groupAccount){
    char sql[1024] = {0};
    sprintf(sql, "select userAccount from groupuser where userAccount != '%s'\
     and groupAccount = '%s'", userAccount.c_str(), groupAccount.c_str());
    ConnectionPool* pool_ = ConnectionPool::getConnectionPool();
    auto mysql = pool_->getConnection(); 
    std::vector<std::string> vec;
    if(mysql != nullptr){
        MYSQL_RES* res = mysql->query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr ){
                vec.push_back(row[0]);
            }
        }
        mysql_free_result(res); 
    }
    return vec;
}