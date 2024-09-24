#include "../../../include/server/Server.hpp"

// as I store the codes as int, if it's an 1 digit number we have to fill the previous 2 digits to 0
// in order to conform with the IRC protocol. I put space as well at the begining and at the end of it
static std::string  getFormatedCode(size_t code)
{
    std::ostringstream ss;
    
    ss << " " << std::setw(3) << std::setfill('0') << code << " ";
    
    return (ss.str());
}

// command string store either the name of a command or a custom message like a nickname or a channelname we couldn't create an object with
std::string Server::getMessage(Client *client, Client *target,
                        Channel *channel, const std::string &command, size_t code)
{
    std::ostringstream      msg;
    const std::set<Client*> &clients = channel->getClients();

    switch (code)
    {
        case RPL_WELCOME:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << RPL_WELCOME_T;
            break;

        case ERR_NONICKNAMEGIVEN:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << ERR_NONICKNAMEGIVEN_T;
            break;

        case ERR_NOTREGISTERED:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << ERR_NOTREGISTERED_T;
            break;

        case ERR_NEEDMOREPARAMS:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << command << ERR_NEEDMOREPARAMS_T;
            break;

        case ERR_ALREADYREGISTERED:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << ERR_ALREADYREGISTERED_T;
            break;

        case ERR_PASSWDMISMATCH:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << ERR_PASSWDMISMATCH_T;
            break;

        case ERR_CHANNELISFULL:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << ERR_CHANNELISFULL_T;
            break;

        case ERR_INVITEONLYCHAN:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << ERR_INVITEONLYCHAN_T;
            break;

        case ERR_BADCHANNELKEY:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << ERR_BADCHANNELKEY_T;
            break;

        case ERR_CHANOPRIVSNEEDED:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << ERR_CHANOPRIVSNEEDED_T;
            break;

        case ERR_NOSUCHNICK:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << command << ERR_NOSUCHNICK_T;
            break;

        case ERR_NOSUCHCHANNEL:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << command << ERR_NOSUCHCHANNEL_T;
            break;

        case ERR_UNKNOWNCOMMAND:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << command << ERR_UNKNOWNCOMMAND_T;
            break;

        case ERR_NICKNAMEINUSE:
            msg << SERVER_PREFIX << getFormatedCode(code) << command << ERR_NICKNAMEINUSE_T;
            break;

        case ERR_USERNOTINCHANNEL:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << target->getNickName() << " " << channel->getName() << ERR_USERNOTINCHANNEL_T;
            break;

        case ERR_ERRONEUSNICKNAME:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getUserName() << " " << command << ERR_ERRONEUSNICKNAME_T;
            break;
        
        case ERR_NOTONCHANNEL:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << ERR_NOTONCHANNEL_T;
            break;

        case ERR_KEYSET:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << ERR_KEYSET_T;
            break;

        case ERR_USERONCHANNEL:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << target->getNickName() << " " << channel->getName() << ERR_USERONCHANNEL_T;
            break;

        case RPL_NOTOPIC:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << RPL_NOTOPIC_T;
            break;

        case RPL_NAMREPLY:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " = " << channel->getName() << " :";
            for (std::set<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
            {
             if (it != clients.begin())
                 msg << " ";
             if (channel->isOperator(*it))
                 msg << "@" << (*it)->getNickName(); 
             else
                 msg << (*it)->getNickName();
            }
            msg << "\r\n";
            break;

        case RPL_ENDOFNAMES:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << RPL_ENDOFNAMES_T;
            break;

        case RPL_CHANNELMODEIS:
            msg << SERVER_PREFIX << " MODE " << channel->getName() << " +";
            if (channel->isInviteOnly()) msg << "i";
            if (channel->hasPassword()) msg << "k";
            if (channel->getUserLimit() != -1) msg << "l";
            msg << "\r\n";
            break;

        case RPL_TOPIC:
            msg << SERVER_PREFIX << getFormatedCode(code) << client->getNickName() << " " << channel->getName() << " :" << channel->getTopic() << "\r\n";
            break;

        case RAW_MODE:
            msg << SERVER_PREFIX << " MODE " << channel->getName() << " +";
            if (channel->isInviteOnly()) msg << "i";
            if (channel->hasPassword()) msg << "k";
            if (channel->getUserLimit() != -1) msg << "l";
            msg << "\r\n";
            break;

        case RAW_JOIN:
            msg << client->getPrefix() << " JOIN " << channel->getName() << "\r\n";
            break;
        
        case RAW_PART:
            msg << client->getPrefix() << " PART " << channel->getName() << " :" << command << "\r\n";
            break;

        case RAW_QUIT:
            msg << client->getPrefix() << " QUIT " << " :" << command;
            break;
        
        case RAW_TOPIC:
            msg << client->getPrefix() << " TOPIC " << channel->getName() << " :" << channel->getTopic() << "\r\n";
            break;

        case ERR_UNKNOWNMODE:
            msg << client->getPrefix() << command << ERR_UNKNOWNMODE_T;
            break;

        default:
            break;
    }
    return (msg.str());
}