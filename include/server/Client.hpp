#pragma once

class Client
{
    public:
        Client();
        Client(int fd);
        // Client (const Client& other);
        // Client& operator=(const Client& other);
        ~Client();

        int get_fd() const;

    private:
        int _fd;
};