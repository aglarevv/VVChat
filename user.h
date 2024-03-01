#ifndef USER_H
#define USER_H

#include <vector>

#include <string>
#include <QMetaType>


typedef enum{
    OFFLINE,
    ONLINE
} State;


struct User{
    std::string account;
    std::string name;
    std::string password;
    std::string state;
    std::string role;
    std::string groupAccount;
    std::string groupName;
    std::string groupDesc;
    bool flag;
};


//class User
//{
//public:
//    User(std::string account = "", std::string name = "", std::string password = "", std::string state = "offline");
//    void setAccount(std::string account) { this->account = account; }
//    void setName(std::string name) { this->name = name; }
//    void setPassword(std::string password) { this->password = password; }
//    void setState(std::string state) { this->state = state;}

//    std::string getAccount() { return this->account; }
//    std::string getName() { return this->name; }
//    std::string getPassword() { return this->password; }
//    std::string getState() { return this->state;}


//    ~User() = default;

//private:
//    std::string account;
//    std::string name;
//    std::string password;
//    std::string state;
//};
Q_DECLARE_METATYPE(User)
#endif // USER_H
