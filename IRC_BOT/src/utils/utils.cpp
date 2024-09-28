#include "../../include/smartboi.hpp"

/**
 * @brief Retrieves the API key from the environment file.
 *
 * This function reads the environment file specified by ENV_PATH and searches for the
 * environment variable specified by ENV_VAR_NAME. If the variable is found, its value
 * is extracted, with surrounding quotes removed, and returned as a string.
 *
 * @return std::string The API key if found and valid, otherwise an empty string.
 *
 * @note The environment variable should be in the format ENV_VAR_NAME="value".
 *       If the API key is less than 3 characters long, an error message is printed
 *       and an empty string is returned.
 *       If the file cannot be opened, an error message is printed and an empty string is returned.
 */
std::string get_API_key()
{
    std::ifstream       file(ENV_PATH);
    std::string         line;
    std::string         api_key;
    std::string         env_variable = ENV_VAR_NAME;
    env_variable += "=";

    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find(env_variable) == 0)
            {
                api_key = line.substr(env_variable.size());
                
                if (api_key.size() < 3)
                {
                    std::cerr << "Couldn't get the API key." << std::endl;
                    return ("");
                }
                else // removing quotes
                {
                    api_key.erase(0, 1);
                    api_key.erase(api_key.size() - 1, 1);
                }
                break;
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Couldn't get the API key." << std::endl;
    }
    return (api_key);
}
