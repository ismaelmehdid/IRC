#include "../../include/client/RegularRole.hpp"
#include "../../include/server/Server.hpp"

RegularRole::RegularRole() : ARole(NULL) {}

RegularRole::RegularRole(Client *client) : ARole(client) {}

RegularRole::RegularRole(const RegularRole& other) : ARole(other._client) {}

RegularRole&    RegularRole::operator=(const RegularRole& other)
{
    if (this != &other)
    {
        ARole::operator=(other);
    }
    return (*this);
}

RegularRole::~RegularRole() {}

RegularRole* RegularRole::clone() const
{
    return (new RegularRole());
}

static void send_permission_denied(int fd, const std::string &nickName)
{
    global_ircserv->socketSend(fd, ":server 481 " + nickName + ": Permission Denied - You're not an IRC operator.\r\n");
}

void RegularRole::kick(const t_IRCCommand &command)
{
    (void)command;
    send_permission_denied(_client->get_fd(), _client->getNickName());
}

void RegularRole::invite(const t_IRCCommand &command)
{
    (void)command;
    send_permission_denied(_client->get_fd(), _client->getNickName());
}

void RegularRole::topic(const t_IRCCommand &command)
{
    (void)command;
    send_permission_denied(_client->get_fd(), _client->getNickName());
}

void RegularRole::mode(const t_IRCCommand &command)
{
    (void)command;
    send_permission_denied(_client->get_fd(), _client->getNickName());
}
