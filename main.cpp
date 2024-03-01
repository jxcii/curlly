#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <queue>
#include <nlohmann/json.hpp>
#include "lib.h"
#include <thread>
#include <mutex>
#include <future>
#include <sstream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

using json = nlohmann::json;
std::queue<json>* threadsForDoing = new std::queue<json>();

void Task(json data){
    //std::mutex myMutex;
    //std::lock_guard<std::mutex> lock(myMutex);
    CURL* curl = curl_easy_init();
    std::future<bool> res = std::async(replyMessage, curl, data);
    while(!res.get()){
    }
    curl_easy_cleanup(curl);
}
int key = 0;
void AnswerManagement(){
    std::vector<std::thread*>* threads = new std::vector<std::thread*>();
    while(1){
        while(key){
            if(!threadsForDoing->empty()){
                while(!threadsForDoing->empty()){
                    if(threads->size() < 32) {
                        threads->push_back(new std::thread(Task, threadsForDoing->front()));
                        threadsForDoing->pop();
                    } else break;
                }
            }
            if(!threads->empty()) 
                for (int c = 0; c< threads->size(); c++) if(threads->at(c)->joinable()) {
                    threads->at(c)->join(); 
                    threads->erase(threads->begin()+c, threads->begin()+c+1);  
                    continue;
                }
        }
    }
}

int main(){
    CURL* curl = curl_easy_init();
    std::thread threadManagement(AnswerManagement);
    int64_t i = 0;
    while(1){
        if(threadsForDoing->size()<32){ 
            if(curl){
                std::stringstream oss;
                oss << i;
                std::string url = "https://api.telegram.org/bot5310616909:AAHqrFdIoW21YPTAtnAxDRzSVJoILL4XjG4/getUpdates?";
                std::string buf = "offset=" + oss.str() + "&limit=1";
                url.append(buf);
                std::string response;
                curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                CURLcode res = curl_easy_perform(curl);
                if(res != CURLE_OK){
                    printf("oshibka = %s", curl_easy_strerror(res));
                }
                json parsedData = json::parse(response);
                if(parsedData["result"].size() > 0){ 
                    key = 0;
                    if(parsedData["result"][0]["message"]["text"]!=NULL) {
                        threadsForDoing->push(parsedData["result"][0]);
                        i = stoi(parsedData["result"][0]["update_id"].dump()) + 1;
                    }
                } else key = 1;
            } 
        } else key = 1;
    }
    curl_easy_cleanup(curl);
}