#include "lib.h"
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <thread>
#include <future>
#include <chrono>

using json = nlohmann::json;
static size_t write_data(char *ptr, size_t size, size_t nmemb, void *user){
    return size * nmemb;
}
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}
std::string DoThis(CURL* curl, std::string url, std::string response = ""){
    curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        printf("oshibka = %s", curl_easy_strerror(res));
    }
    return response;
}

void editTextBetweenSeconds(std::string text, std::string message_id, std::string chat_id){
    CURL* curl = curl_easy_init();
    if(curl){
        std::string url = "https://api.telegram.org/bot5310616909:AAHqrFdIoW21YPTAtnAxDRzSVJoILL4XjG4/editMessageText?", buf;
        buf = "chat_id=" + chat_id + "&message_id=" + message_id + "&text=" + text + "&parse_mode=MarkdownV2";
        DoThis(curl, (url+buf));
        curl_easy_cleanup(curl);
    }
}
using json = nlohmann::json;

std::string urlencode(const std::string &s)
{
    static const char lookup[]= "0123456789abcdef";
    std::stringstream e;
    for(int i=0, ix=s.length(); i<ix; i++)
    {
        const char& c = s[i];
        if ( (48 <= c && c <= 57) ||//0-9
             (65 <= c && c <= 90) ||//abc...xyz
             (97 <= c && c <= 122) || //ABC...XYZ
             (c=='_' || c=='~') 
        )
        {
            e << c;
        }
        else if (c == '.' || c == '('|| c == ')' || c == '-') {
            e << '\\';
            e << c;
        }
        else
        {
            e << '%';
            e << lookup[ (c&0xF0)>>4 ];
            e << lookup[ (c&0x0F) ];
        }
    }
    return e.str();
}
bool replyMessage(CURL* curl, json data){
    if(curl){
        std::string url = "https://api.telegram.org/bot5310616909:AAHqrFdIoW21YPTAtnAxDRzSVJoILL4XjG4/sendMessage?", buf, chat_id = data["message"]["chat"]["id"].dump();
        printf("1\n");
        printf("%s\n", data["message"]["text"].dump().substr(0, 8).c_str());
        if(data["message"]["text"].dump()=="\"/start\""){
            buf = "chat_id=" + chat_id + "&text=" + "Assalomu%20alaykum%20%20%20\U0001F508" + "&parse_mode=MarkdownV2";
            json info = json::parse(DoThis(curl, (url + buf)));
            for(int c = 0; c<8; c++){
                if(c%3==0) editTextBetweenSeconds("Assalomu%20alaykum%20%20%20\U0001F509", info["result"]["message_id"].dump(), info["result"]["chat"]["id"].dump());
                if(c%3==1) editTextBetweenSeconds("Assalomu%20alaykum%20%20%20\U0001F50A", info["result"]["message_id"].dump(), info["result"]["chat"]["id"].dump());
                if(c%3==2) editTextBetweenSeconds("Assalomu%20alaykum%20%20%20\U0001F508", info["result"]["message_id"].dump(), info["result"]["chat"]["id"].dump());
            }
            buf = "chat_id=" + chat_id + "&text=" + "Ma'lumot%20sifatida%20bilib%20olishingiz%20kerak:" + "&parse_mode=MarkdownV2";
            DoThis(curl, (url + buf));
            buf = "chat_id=" + chat_id + "&text=" + "\U00000031\U0000FE0F\U000020E3%20Maqsadim%20ingliz%20tilini%20o%27rganishga%20yordam%20beruvchi%20[Oxford%20English%20Dictionary%20](https://www.oed.com/?tl=true)saytidan%20olinadigan%20so%27zlar%20bazasi%20yordamida%20universal%20quiz%20testlarni%20%7c%7calgoritm%20yordamida%7c%7c%20yasab%20%22challenge%22lar%20o%27tkazish%2C%20sizlarga%20ulashish%20va%20xohishingiz%20bo%27yicha%20taqdim%20etish" + "&parse_mode=MarkdownV2";
            info = json::parse(DoThis(curl, (url + buf)));
        }
        if(data["message"]["text"].dump().substr(0, 8) == "\"search:"){
            url = "https://oed-researcher-api.oxfordlanguages.com/oed/api/v0.2/senses/?lemma=";
            url.append(urlencode(data["message"]["text"].dump().substr(9, data["message"]["text"].dump().length()-9-1)));
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "accept: application/json");
            headers = curl_slist_append(headers, "app_id: e5ff3d39");
            headers = curl_slist_append(headers, "app_key: cfd13f21324edbdc017ecf6d66ff2424");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            json info = json::parse(DoThis(curl, url));
            curl_slist_free_all(headers);
            std::string url = "https://api.telegram.org/bot5310616909:AAHqrFdIoW21YPTAtnAxDRzSVJoILL4XjG4/sendMessage?";
            buf = "chat_id=" + chat_id + "&text=";
            printf("%s\n", info.dump());
            if(info["data"].size() > 0){
                curl_easy_cleanup(curl);
                curl = curl_easy_init();
                for(int c=0; c<info["data"].size(); c++){
                    if(info["data"][c]["definition"]!=NULL) {
                        buf += urlencode(info["data"][c]["definition"].dump().substr(1, info["data"][c]["definition"].dump().length()-2)) + std::string("%0A");
                        buf += "&parse_mode=MarkdownV2";
                        DoThis(curl, (url+buf));
                        buf = "chat_id=" + chat_id + "&text=";
                    }
                }
            }
        }
    }
    printf("2\n");
    return 1;
}