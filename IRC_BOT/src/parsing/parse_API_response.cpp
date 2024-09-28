#include "../../include/smartboi.hpp"

/**
 * @brief Extracts the value of a specified element from a raw JSON-like string.
 *
 * This function searches for a given element within a raw string and extracts its value.
 * If the element is not found, it returns "No data". For the "description" element, it
 * trims the surrounding quotes from the extracted value.
 *
 * @param element The name of the element to search for in the raw string.
 * @param raw The raw JSON-like string from which to extract the element's value.
 * @return The value of the specified element, or "No data" if the element is not found.
 */
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

/**
 * @brief Parses the raw weather API response and extracts weather information for a given location.
 *
 * This function takes a raw API response string and a location string, then extracts and formats
 * weather information such as conditions, temperature, and humidity. If the HTTP response code
 * is not "200", it returns a "Location not found." message.
 *
 * @param raw The raw API response string.
 * @param location The location for which the weather information is being parsed.
 * @return A formatted string containing the weather information for the given location.
 */
std::string parse_weather_api_response(const std::string &raw, const std::string &location)
{
    std::string parsed;
    struct t_weather_infos weather;

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
