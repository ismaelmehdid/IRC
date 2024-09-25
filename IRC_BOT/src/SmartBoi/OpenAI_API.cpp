#include "../../include/SmartBoi/SmartBoi.hpp"
#include "../../include/smartboi.hpp"

// This function is called everytime curl receive a chunk of data from the API.
// It append the data received to the string userp and return the size of the data received
static size_t WriteCallback(void* received, size_t size, size_t nbr_of_char, void* buffer) {
    ((std::string*)buffer)->append((char*)received, size * nbr_of_char);
    return size * nbr_of_char;
}

std::string SmartBoi::call_openai_api(const std::string &request)
{
    CURL        *curl;
    CURLcode    res;
    std::string buffer;

    curl = curl_easy_init();
    if (curl) {

        // -------------------------------------------------------
        // set the http request datas, CURLOPT_URL is a flag to say that we will provide the URL of the https request
        curl_easy_setopt(curl, CURLOPT_URL, OPENAI_API_HOST);
        // -------------------------------------------------------

        // -------------------------------------------------------
        // creating json part of the request with all the necessary data
        std::string json_request = "{"
            "\"model\": \"gpt-3.5-turbo\", "
            "\"messages\": [{\"role\": \"user\", \"content\": \"" + request + "\"}], "
            "\"max_tokens\": 50"
        "}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_request.c_str()); // adding the json request
        // -------------------------------------------------------

        // -------------------------------------------------------
        // adding the HTTP header to the request, this part:
        //   -H "Content-Type: application/json"
        //   -H "Authorization: Bearer YOUR_API_KEY"
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string authHeader = "Authorization: Bearer " + _api_key;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // adding the HTTP header to the request
        // -------------------------------------------------------

        // By default curl return the response on the standard output, so we give him a function to write everything in a buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);  // send the request

        if (res != CURLE_OK) { // request failed
            std::cerr << "Failed to communicate with the OPENAI API: " << curl_easy_strerror(res) << std::endl;
        } else {
            return buffer;
        }
        curl_easy_cleanup(curl);
        std::cout << buffer << std::endl;
    } else {
        throw std::runtime_error("Failed to init curl.");
    }
    return "";
}

// Example of a curl request to the OPENAI API
// curl https://api.openai.com/v1/chat/completions
//   -H "Content-Type: application/json"
//   -H "Authorization: Bearer YOUR_API_KEY"
//   -d '{
//     "model": "gpt-3.5-turbo",
//     "messages": [{"role": "user", "content": "Hey, how are you ?"}],
//     "max_tokens": 50
//   }'
