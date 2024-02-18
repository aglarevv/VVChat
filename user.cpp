#include "user.h"

User::User(std::string account, std::string name, std::string password, std::string state)
{
    this->account = account;
    this->name = name;
    this->password = password;
    this->state = state;
}
