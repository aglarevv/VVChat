#include <iostream>
#include "liboai.h"

using namespace liboai;
int main(){
    OpenAI oai;
    Conversation convo;
    convo.SetSystemData("You are a helpful bot that only answers questions about OpenAI.");
    convo.AddUserData("介绍一下你自己");
    if (oai.auth.SetKey("sk-ejiCK8hRfa8mwM0V3GHGYWEcNwW6rQYfimNsgfuWdXKo93YM")) { // NOT recommended
        try {
            Response response = oai.ChatCompletion->create("gpt-3.5-turbo", convo);
            convo.Update(response);  
            std::cout << convo.GetLastResponse() << std::endl;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }




}