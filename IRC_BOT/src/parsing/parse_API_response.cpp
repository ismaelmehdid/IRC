#include "../../include/smartboi.hpp"

static std::string get_element(const std::string &element, const std::string &raw)
{
    std::string result;

    size_t pos = raw.find(element);
    if (pos == std::string::npos) {
        return "No data";
    }
    pos += element.size() + 2;
    
    while (pos < raw.size() && raw[pos] != ',' && raw[pos] != '}') {
        result += raw[pos];
        pos++;
    }

    if (element == "description") {
        if (result.size() > 2) {
            result = result.substr(1, result.size() - 2);
        }
    }

    return result;
}

std::string parse_weather_api_response(const std::string &raw, const std::string &location)
{
    std::string parsed;
    struct t_weather_infos weather;
    std::memset(&weather, 0, sizeof(weather));

    std::stringstream iss(raw);
    std::string http_code;
    iss >> http_code;
    iss >> http_code;
    
    if (http_code != "200") {
        return "Location not found.";
    }

    weather.location = location;
    weather.conditions = get_element("description", raw);
    weather.temperature = get_element("temp", raw);
    weather.humidity = get_element("humidity", raw);

    parsed += "In " + location + ", ";
    parsed += "Conditions: " + weather.conditions + ", ";
    parsed += "Temperature: " + weather.temperature + (weather.temperature != "No data" ? "°C" : "") + ", ";
    parsed += "Humidity: " + weather.humidity + (weather.humidity != "No data" ? "%" : "");

    return parsed;
}
