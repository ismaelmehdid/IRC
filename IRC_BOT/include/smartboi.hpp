#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <curl/curl.h>
#include <cstring>
#include <vector>
#include <map>
#include <exception>
#include <sstream>

//===----------------------------------------------------------------------===//

#define EXPECTED_NBR_OF_ARGS    4

#define NICK_NAME               "smartboi"

#define ENV_PATH                "../.env"    // To change if needed
#define ENV_VAR_NAME            "API_KEY"   // To change if needed

#define WEATHER_API_HOST        "api.openweathermap.org" // following the path of the request
#define OPENAI_API_HOST         "https://api.openai.com/v1/chat/completions"
#define HTTP_PORT               80

//===----------------------------------------------------------------------===//

class SmartBoi;

//===----------------------------------------------------------------------===//

extern SmartBoi *global_smartboi;

//===----------------------------------------------------------------------===//

struct t_IRCCommand
{
    std::string                 prefix;
    std::string                 command;
    std::vector<std::string>    params;
    std::string                 trailing;
};

struct t_weather_infos
{
    std::string location;       // paris, nyc, ...
    std::string temperature;    // x degres
    std::string conditions;     // cloudy, sunny, ...
    std::string humidity;       // x%
};

//===----------------------------------------------------------------------===//

std::vector<t_IRCCommand>   parseRequests(const std::string &requests);
std::string                 get_API_key();
std::string                 parse_weather_api_response(const std::string &raw, const std::string &location);
std::string                 parse_openai_api_response(const std::string& response);

//===----------------------------------------------------------------------===//