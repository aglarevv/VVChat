#pragma once

#include <vector>
#include <string>



enum type {
    USER = 0,
    GROUP = 1
};

//生成随机且不重复账号类
class RandomAccountGenerator {
public:
    RandomAccountGenerator(type account = USER);
    //生成随机账号
    std::string generateUniqueAccount();


private:
    //已生成的账号
    std::vector<bool> bitset;
    //映射为哈希值
    std::hash<std::string> hashFn;
    
    bool isUnique(const std::string& account) const;

    void markAsUsed(const std::string& account);

    type m_accountType;

};
