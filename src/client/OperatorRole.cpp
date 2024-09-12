#include "../../include/client/OperatorRole.hpp"

OperatorRole::OperatorRole() {}

OperatorRole::OperatorRole(const OperatorRole& other)
{
    (void)other; // nothing to copy
}

OperatorRole&   OperatorRole::operator=(const OperatorRole& other)
{
    (void)other; // nothing to copy
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

void OperatorRole::pass(const t_IRCCommand &command)
{
    (void)command;
}

void OperatorRole::nick(const t_IRCCommand &command)
{
    (void)command;
}

void OperatorRole::user(const t_IRCCommand &command)
{
    (void)command;
}
