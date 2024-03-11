#ifndef LIB_H
#define LIB_H
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
bool replyMessage(CURL* curl, std::string data);

#endif
