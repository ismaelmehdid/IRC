#include "../../include/client/RegularRole.hpp"

RegularRole::RegularRole() : ARole(NULL) { }

RegularRole::RegularRole(Client *client) : ARole(client) { }

RegularRole::RegularRole(const RegularRole& other) : ARole(other._client) {}

RegularRole&    RegularRole::operator=(const RegularRole& other)
{
    if (this != &other) {
        ARole::operator=(other);
    }
    return (*this);
}

RegularRole::~RegularRole() {}

RegularRole* RegularRole::clone() const
{
    return new RegularRole();
}

void RegularRole::kick(const t_IRCCommand &command)
{
    (void)command;
    // send permission error
}

void RegularRole::invite(const t_IRCCommand &command)
{
    (void)command;
    // send permission error
}

void RegularRole::topic(const t_IRCCommand &command)
{
    (void)command;
    // send permission error
}

void RegularRole::mode(const t_IRCCommand &command)
{
    (void)command;
    // send permission error
}
