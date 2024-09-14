#include "../../include/client/OperatorRole.hpp"

OperatorRole::OperatorRole() : ARole(NULL) { }

OperatorRole::OperatorRole(Client *client) : ARole(client) { }

OperatorRole::OperatorRole(const OperatorRole& other) : ARole(other._client) { }

OperatorRole&   OperatorRole::operator=(const OperatorRole& other)
{
    if (this != &other) {
        ARole::operator=(other);
    }
    return (*this);
}

OperatorRole::~OperatorRole() {}

OperatorRole* OperatorRole::clone() const
{
    return new OperatorRole();
}

void OperatorRole::kick(const t_IRCCommand &command)
{
    (void)command;
}

void OperatorRole::invite(const t_IRCCommand &command)
{
    (void)command;
}

void OperatorRole::topic(const t_IRCCommand &command)
{
    (void)command;
}

void OperatorRole::mode(const t_IRCCommand &command)
{
    (void)command;
}
