#pragma once

#include "../smartboi.hpp"

class SmartBoi
{
    private:
        SmartBoi(const SmartBoi &toCopy);
        SmartBoi &operator=(const SmartBoi &toCopy);

        void        connect_to_irc_server(const std::string &server_ip, const std::string &server_port, const std::string &password);
        void        loop();

        std::string call_weather_api(const std::string &request);
        void        connect_to_weather_api();

        void        handle_response(const std::string &message);


        int         _irc_socket_fd;
        int         _api_socket_fd;
        std::string _api_key;

    public:
        SmartBoi();
        ~SmartBoi();

        void        start(const std::string &server_ip, const std::string &server_port, const std::string &server_password);
        int         get_irc_socket_fd() const;

};