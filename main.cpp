#include "lib.h"
#include <stdio.h>
#include <curl/curl.h>
#include <queue>
#include <future>
#include <thread>
#include <sstream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::queue<std::string>* threadsForDoing = new std::queue<std::string>();
int key = 0;
void Task(std::string data){
    //std::mutex myMutex;
    //std::lock_guard<std::mutex> lock(myMutex);
    CURL* curl = curl_easy_init();
    replyMessage(curl, data);
    curl_easy_cleanup(curl);
    
}

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
                rapidjson::Document parsedData;
                printf("%s\n", url.c_str());
                printf("%s\n", response.c_str());
                parsedData.Parse(response.c_str());
                printf("%d-kol\n", parsedData["result"].Size());
                int size = parsedData["result"].Size();
                if(size > 0){ 
                    key = 0;
                    printf("%d-dol\n", parsedData["result"][0]["message"]["text"].IsString());
                    if(parsedData["result"][0]["message"]["text"].IsString()) {
                        rapidjson::Document copy;
                        copy.CopyFrom(parsedData["result"][0], copy.GetAllocator());
                        rapidjson::StringBuffer buffer;
                        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        copy.Accept(writer);
                        std::string jsonString = buffer.GetString();
                        printf("info-%s\n" , jsonString.c_str());
                        threadsForDoing->push(jsonString);
                        int number = parsedData["result"][0]["update_id"].GetInt();
                        i = number + 1;
                    }
                } else key = 1;
            } 
        } else key = 1;
    }
    curl_easy_cleanup(curl);
}