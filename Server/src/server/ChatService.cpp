#include "ChatService.hpp"


#include <iostream>

//对外实例化方法
ChatService* ChatService::instance(){
    static ChatService server;
    return &server;
}
//构造
ChatService::ChatService(){
    //登录
    handleMap_.insert(
        {LOGIN, std::bind(&ChatService::login, this,
         std::placeholders::_1, 
         std::placeholders::_2, 
         std::placeholders::_3)}
    );
    //登出
    handleMap_.insert(
        {LOGINOUT, std::bind(&ChatService::loginout, this,
         std::placeholders::_1, 
         std::placeholders::_2, 
         std::placeholders::_3)}
    );
    //注册
    handleMap_.insert(
        {REGISTER, std::bind(&ChatService::regist, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)}
    );
    //一对一聊天
    handleMap_.insert(
        {ONE_CHAT, std::bind(&ChatService::oneChat, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)}
    );
    //添加好友
    handleMap_.insert(
        {FRIEDN_ADD, std::bind(&ChatService::addFriend, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)}
    );
    //创建群组
    handleMap_.insert(
        {GROUP_CREATE, std::bind(&ChatService::createGroup, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)}
    );
    //加入群组
    handleMap_.insert(
        {GROUP_ADD, std::bind(&ChatService::addGroup, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)}
    );
    //群组聊天
    handleMap_.insert(
        {GROUP_CHAT, std::bind(&ChatService::chatGroup, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3)}
    );
    //redis
    if(redis_.connect()){
        redis_.initNotifyMessageHandle(std::bind(&ChatService::redisHandleMessage, this,
        std::placeholders::_1,
        std::placeholders::_2));
    }
    //AIChat
    // handleMap_.insert(
    //     {AI_CHAT, std::bind(&ChatService::AICHAT, this,
    //     std::placeholders::_1)}
    // );
    
}
//登录
void ChatService::login(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string account = js["account"];
    std::string password = js["password"];
    User user = userModel_->query(account);
    if(user.getPassword() == password && (account != "")){
        //登录成功
        if(user.getState() == "online"){
            //重复登录
            json response;
            response["msgType"] = LOGIN_ACK;
            response["errorNo"] = 1;
            response["errorMsg"] = "该用户已登录";
            conn->send(response.dump());
        }
        else{
            //首次登录
            {
                std::unique_lock<std::mutex> lock(mutex_);
                userConnMap_.insert({account, conn});
            }
            redis_.subscribe(account);
            user.setState("online");
            userModel_->updataState(user);
            json response;
            response["msgType"] = LOGIN_ACK;
            response["errorNo"] = 0;
            response["account"] = user.getAccount();
            response["name"] = user.getName();
            //获取离线消息
            std::vector<std::string> offlineMessageVec = OfflineMessageModel_->query(account);
            if(!offlineMessageVec.empty()){
                response["offlineMessage"] = offlineMessageVec;
                OfflineMessageModel_->remove(account);
            }
            //获取好友信息
            auto friendVec = friendModel_->query(account);
            if(!friendVec.empty()){
                std::vector<std::string> vec;
                for(auto& user : friendVec){
                    json js;
                    js["account"] = user.getAccount();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec.push_back(js.dump());
                }
                response["friends"] = vec;
            }
            //获取群组信息
            std::vector<AllGroup> groupVec = groupModel_->queryGroups(account);
            if(!groupVec.empty()){
                //群组
                std::vector<std::string> groupV;
                for(AllGroup& group : groupVec){
                    json groupjs;
                    groupjs["groupAccount"] = group.getGroupAccount();
                    groupjs["groupName"] = group.getGroupName();
                    groupjs["groupDesc"] = group.getGroupDesc();
                    //群员
                    std::vector<std::string> userV;
                    for(GroupUser& user : group.getUsers()){
                        json js;
                        js["account"] = user.getAccount();
                        js["name"] = user.getName();
                        js["state"] = user.getState();
                        js["role"] = user.getRole();
                        userV.push_back(js.dump());
                    }
                    groupjs["users"] = userV;
                    groupV.push_back(groupjs.dump());
                }
                response["groups"] = groupV;
            }
            conn->send(response.dump());
        }
    }
    //登录失败
    else{
        json response;
        response["msgType"] = LOGIN_ACK;
        response["errorNo"] = 1;
        response["errorMsg"] = "登录失败,用户名或密码错误";
        conn->send(response.dump());
    }
}
//登出
void ChatService::loginout(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string account = js["account"];
    {
        std::unique_lock<std::mutex> lock(mutex_);
        auto it = userConnMap_.find(account);
        if(it != userConnMap_.end()){
            userConnMap_.erase(it);
        }
    }
    redis_.unSubscribe(account);
    //断开服务器连接
    User user;
    user.setName(account);
    userModel_->updataState(user);
}
//注册
void ChatService::regist(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string name = js["name"];
    std::string password = js["password"];
    User user;
    user.setName(name);
    user.setPassword(password);
    bool state = userModel_->insert(user);
    if(state){
        //注册成功
        json response;
        response["msgType"] = REGISTER_ACK;
        response["errorNo"] = 0;
        response["account"] = user.getAccount();
        conn->send(response.dump());
    }
    else{
        //注册失败
        json response;
        response["msgType"] = REGISTER_ACK;
        response["errorNo"] = 1;
        response["errorMsg"] = "注册失败";
        conn->send(response.dump());
    }

}
//一对一聊天
void ChatService::oneChat(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
     std::string sendToAccount = js["sendToAccount"];
    // if(name == "AICHAT")
    // {
    //     convo.SetSystemData("You are a helpful bot that only answers questions about OpenAI.");
    //     convo.AddUserData(js.["msg"]);
    //     if (oai.auth.SetKey("sk-ejiCK8hRfa8mwM0V3GHGYWEcNwW6rQYfimNsgfuWdXKo93YM")) { // NOT recommended
    //         try {
    //             Response response = oai.ChatCompletion->create("gpt-3.5-turbo", convo);
    //             convo.Update(response);  
    //             str = convo.GetLastResponse();
    //         }
    //         catch (std::exception& e) {
    //             std::cout << e.what() << std::endl;
    //         }
    //     }
    // }
    //同一主机
    {
        std::unique_lock<std::mutex> lock(mutex_);
        auto it = userConnMap_.find(sendToAccount);
        if(it != userConnMap_.end()){
            //在线，转发消息
            it->second->send(js.dump());
            return;
        }
    }
    //不同主机
    User user = userModel_->query(sendToAccount);
    if(user.getState() == "online"){
        redis_.publish(user.getName(), js.dump());
        return;
    }
    //不在线
    OfflineMessageModel_->insert(sendToAccount, js.dump());
}
//添加好友
void ChatService::addFriend(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string userAccount  = js["userAccount"];
    std::string friendAccount = js["friendAccount"];
    json responsejs;
    if(friendModel_->insert(userAccount, friendAccount)){
        responsejs["msgType"] = GROUP_CREATE_ACK;
        responsejs["errorNo"] = 0;
        responsejs["errorMsg"] = "添加成功";
        conn->send(responsejs.dump());
    }
    else{
        responsejs["msgType"] = GROUP_CREATE_ACK;
        responsejs["errorNo"] = 1;
        responsejs["errorMsg"] = "添加失败";
        conn->send(responsejs.dump());
    }
    
}
//创建群组
void ChatService::createGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string account = js["account"];
    std::string groupAccount = js["groupAccount"];
    std::string desc = js["groupdesc"];
    AllGroup group(groupAccount, desc);
    json responsejs;
    if(groupModel_->create(group)){
        groupModel_->add(group.getGroupAccount(), account, "creator");
        responsejs["msgType"] = GROUP_CREATE_ACK;
        responsejs["errorNo"] = 0;
        responsejs["errorMsg"] = "创建成功";
        conn->send(responsejs.dump());
    }
    else{
        responsejs["msgType"] = GROUP_CREATE_ACK;
        responsejs["errorNo"] = 1;
        responsejs["errorMsg"] = "创建失败";
        conn->send(responsejs.dump());
    }
}
//加入群组
void ChatService::addGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string account = js["account"];
    std::string groupAccount = js["groupAccount"];
    bool res = groupModel_->add(groupAccount, account, "normal");
    json responsejs;
    if(!res){
        responsejs["msgType"] = GROUP_ADD_ACK;
        responsejs["erroeNo"] = 1;
        responsejs["account"] = account;
        responsejs["groupAccount"] = groupAccount;
        responsejs["errorNsg"] = "加入失败，该群组不存在或已加入";
        conn->send(responsejs.dump());
    }
    else{
        responsejs["msgType"] = GROUP_ADD_ACK;
        responsejs["erroeNo"] = 0;
        responsejs["account"] = account;
        responsejs["groupAccount"] = groupAccount;
        responsejs["errorNsg"] = "加入成功";
        conn->send(responsejs.dump());
    }
   
}
//群组聊天
void ChatService::chatGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time){
    std::string account = js["account"];
    std::string groupid = js["groupAccount"];
    std::vector<std::string> vec = groupModel_->queryUsers(account, groupid);
    std::unique_lock<std::mutex> lock(mutex_);
    for(auto user : vec){
        auto it = userConnMap_.find(user);
        if(it != userConnMap_.end()){
            it->second->send(js.dump());
        }
        else{
            //不同主机
            User u = userModel_->query(user);
            if(u.getState() == "online"){
                redis_.publish(u.getName(), js.dump());
            }
            else{
                OfflineMessageModel_->insert(user, js.dump());
            }
        }
    }
}
//redis
void ChatService::redisHandleMessage(std::string channel, std::string message){
    std::unique_lock<std::mutex> lock(mutex_);
    auto it = userConnMap_.find(channel);
    if(it != userConnMap_.end()){
        it->second->send(message);
        return;
    }
    OfflineMessageModel_->insert(channel, message);
}
//AICHAT
// string response(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time)
// {
    
// }

//业务分发
ChatService::MsgHandle ChatService::getHandle(int msgType){
    //记录错误日志
    auto it = handleMap_.find(msgType);
    if(it == handleMap_.end()){
        return [=](const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp){
            LOG_ERROR << "msgType: " << msgType << " getHandl error";
        };
    }
    else{
        //分发不同消息的处理函数
        return handleMap_[msgType];
    }
}
//客户端连接断开异常
void ChatService::clientCloseException(const muduo::net::TcpConnectionPtr& conn){
    User user;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        for(auto it = userConnMap_.begin(); it != userConnMap_.end(); ++it){
            if(it->second == conn){
                user.setName(it->first);
                userConnMap_.erase(it);
                break;
            }
        }
    }
    redis_.unSubscribe(user.getName());
    //断开服务器连接
    if(user.getName() != ""){
        user.setState("offline");
        userModel_->updataState(user);
    }
}
//服务器连接断开异常
void ChatService::reset(){
    //重置客户端状态
    userModel_->resetState();
}