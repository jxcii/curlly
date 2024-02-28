#include "lib.h"
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
using json = nlohmann::json;
static size_t write_data(char *ptr, size_t size, size_t nmemb, void *user){
    return size * nmemb;
}
using json = nlohmann::json;
bool replyMessage(CURL* curl, json data){
    if(curl){
        //if(data["result"][0]["message"]["text"].dump()=="\"/check\""){
            std::string chat_id = "637360098";//data["message"]["chat"]["id"].dump();
            printf("4\n");
            std::string url = "https://api.telegram.org/bot5310616909:AAHqrFdIoW21YPTAtnAxDRzSVJoILL4XjG4/sendMessage?", response;
            std::string buf = "chat_id=" + chat_id + "&text=" + "\U0001F636\U0000200D\U0001F32B\U0000FE0F" + "&parse_mode=MarkdownV2";
            url.append(buf);
            curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK){
                printf("oshibka = %s", curl_easy_strerror(res));
            }
            printf("5\n");
        //}
    }
    return 1;
}
