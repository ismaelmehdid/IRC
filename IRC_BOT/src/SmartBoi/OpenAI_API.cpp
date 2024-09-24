#include "../../include/SmartBoi/SmartBoi.hpp"
#include "../../include/smartboi.hpp"

// size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
//     ((std::string*)userp)->append((char*)contents, size * nmemb);
//     return size * nmemb;
// }

// std::string extractContent(const std::string& response) {
//     const std::string key = "\"content\": \"";
//     size_t start = response.find(key);
//     if (start == std::string::npos) {
//         return "";
//     }
//     start += key.length();

//     size_t end = response.find("\"", start);
//     if (end == std::string::npos) {
//         return "";
//     }

//     return response.substr(start, end - start);
// }

// void call_llm_api(const std::string &api_key, const std::string &input)
// {
//     CURL        *curl;
//     CURLcode    res;
//     std::string buffer;

//     curl = curl_easy_init();
//     if (curl) {
//         curl_easy_setopt(curl, CURLOPT_URL, OPENAI_API_URL);

//         std::string json_request = "{"
//             "\"model\": \"gpt-3.5-turbo\", "
//             "\"messages\": [{\"role\": \"user\", \"content\": \"" + input + "\"}], "
//             "\"max_tokens\": 50"
//         "}";

//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_request.c_str());

//         // adding the API header to the HTTP request
//         struct curl_slist* headers = NULL;
//         headers = curl_slist_append(headers, "Content-Type: application/json");
//         std::string authHeader = "Authorization: Bearer " + api_key;
//         headers = curl_slist_append(headers, authHeader.c_str());
//         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

//         res = curl_easy_perform(curl);

//         if (res != CURLE_OK) {
//             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//         } else {
//             std::string extractedContent = extractContent(buffer);
//             std::cout << extractedContent << std::endl;
//         }
//         curl_easy_cleanup(curl);
//     }

// }