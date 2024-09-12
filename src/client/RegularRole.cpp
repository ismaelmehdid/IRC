#include "../../include/client/RegularRole.hpp"

RegularRole::RegularRole() {}

RegularRole::RegularRole(const RegularRole& other)
{
    (void)other; // nothing to copy
}

RegularRole&    RegularRole::operator=(const RegularRole& other)
{
    (void)other; // nothing to copy
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

void RegularRole::pass(const t_IRCCommand &command)
{
    (void)command;

}

void RegularRole::nick(const t_IRCCommand &command)
{
    (void)command;

}

void RegularRole::user(const t_IRCCommand &command)
{
    (void)command;

}
