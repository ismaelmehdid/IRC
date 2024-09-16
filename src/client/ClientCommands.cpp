#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"

void    Client::executeKick(const t_IRCCommand &command)
{
    this->_role->kick(command);
}

void    Client::executeInvite(const t_IRCCommand &command)
{
    this->_role->invite(command);
}

void    Client::executeTopic(const t_IRCCommand &command)
{
    this->_role->topic(command);
}

void    Client::executeMode(const t_IRCCommand &command)
{
    this->_role->mode(command);
}

void    Client::executeCap(const t_IRCCommand &command)
{
    this->_role->cap(command);
}

void    Client::executePass(const t_IRCCommand &command)
{
    this->_role->pass(command);
}

void    Client::executeNick(const t_IRCCommand &command)
{
    this->_role->nick(command);
}

void    Client::executeUser(const t_IRCCommand &command)
{
    this->_role->user(command);
}

void    Client::executeJoin(const t_IRCCommand &command)
{
    this->_role->join(command);
}

void    Client::executePart(const t_IRCCommand &command)
{
    this->_role->part(command);
}

void    Client::executePrivMsg(const t_IRCCommand &command)
{
    this->_role->privMsg(command);
}

void    Client::executePing (const t_IRCCommand &command)
{
    this->_role->ping(command);
}