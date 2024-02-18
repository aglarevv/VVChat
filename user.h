#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
    User(std::string account = "", std::string name = "", std::string password = "", std::string state = "offline");
    void setAccount(std::string account) { this->account = account; }
    void setName(std::string name) { this->name = name; }
    void setPassword(std::string password) { this->password = password; }
    void setState(std::string state) { this->state = state;}

    std::string getAccount() { return this->account; }
    std::string getName() { return this->name; }
    std::string getPassword() { return this->password; }
    std::string getState() { return this->state;}


    ~User() = default;

private:
    std::string account;
    std::string name;
    std::string password;
    std::string state;
};

#endif // USER_H
