#ifndef LIB_H
#define LIB_H
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
bool replyMessage(CURL* curl, json data);

#endif
