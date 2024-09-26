#include "../../include/smartboi.hpp"

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
