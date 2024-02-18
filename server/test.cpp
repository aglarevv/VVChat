#include <curl/curl.h>
#include <iostream>
#include <string>
#include "json.hpp"
using json = nlohmann::json ;
#include "liboai"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

int main(){
   std::string userInput;
   std::getline(std::cin, userInput);
   CURL *curl;
   CURLcode res;
   curl = curl_easy_init();
   if(curl) {
         curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
         curl_easy_setopt(curl, CURLOPT_URL, "https://api.chatanywhere.com.cn/v1/chat/completions");
         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
         curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
         struct curl_slist *headers = NULL;
         headers = curl_slist_append(headers, "Authorization: Bearer sk-ejiCK8hRfa8mwM0V3GHGYWEcNwW6rQYfimNsgfuWdXKo93YM");
         headers = curl_slist_append(headers, "User-Agent: Apifox/1.0.0 (https://apifox.com)");
         headers = curl_slist_append(headers, "Content-Type: application/json");
         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
         json requestData = {
               {"model", "gpt-3.5-turbo"},
               {"messages", {
                  {"role", "user"},
                  {"content", userInput}
               }}
         };
         const char *data = requestData.dump().c_str();
         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

         std::string apiResponse;
         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiResponse);

         res = curl_easy_perform(curl);

         if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
         } else {
            // 解析JSON响应
            try {
                  json jsonResponse = json::parse(apiResponse);
                  // 使用nlohmann/json的功能来获取内容等信息
                  std::string content = jsonResponse["choices"][0]["message"]["content"];
                  std::cout << "Content: " << content << std::endl;
            } catch (const std::exception &e) {
                  std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
         }

   }
   curl_easy_cleanup(curl);
}
