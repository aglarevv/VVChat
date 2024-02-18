#include "other/randomAccountGenerator.hpp"

#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include <functional>
#include <ctime>
#include <chrono>


//随机数生成
std::string generateRandomAccount(type m_accountType) {
    
    //账号的字符集
    const int accountLength = m_accountType;
    const std::string charset = "0123456789";
    std::string account;
    account.reserve(accountLength);
    //随机数种子
    std::random_device rd;
    //随机数生成器
    std::mt19937 gen(rd());
    //均匀分布的整数，选择字符集中的一个索引
    std::uniform_int_distribution<int> distribution(0, charset.size());

    for (int i = 0; i < accountLength; ++i) {
        account += charset[distribution(gen)];
    }
    return account;
    
    

    
}
RandomAccountGenerator* RandomAccountGenerator::instance(type account)
{
    static RandomAccountGenerator radm(account);
    return &radm;
}

//生成随机且不重复账号类
RandomAccountGenerator::RandomAccountGenerator(type account) : bitset(1 << 20), hashFn(std::hash<std::string>()) {
    m_accountType = account;
}
//生成随机账号
std::string RandomAccountGenerator::generateUniqueAccount() {
    std::string newAccount;
    do {
        newAccount = generateRandomAccount(m_accountType);
    } while (!isUnique(newAccount));
    //标记账号为已使用
    markAsUsed(newAccount);
    return newAccount;
}

//检查账号是否已经生成过   
bool RandomAccountGenerator::isUnique(const std::string& account) const {
    size_t hashValue = hashFn(account);
    return !bitset[hashValue & (bitset.size() - 1)];
}
//将账号标记为已使用
void RandomAccountGenerator::markAsUsed(const std::string& account) {
    size_t hashValue = hashFn(account);
    bitset[hashValue & (bitset.size() - 1)] = true;
}
