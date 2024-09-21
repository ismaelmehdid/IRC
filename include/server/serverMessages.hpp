#define SERVER_NAME "server"

#define SERVER_PREFIX ":" SERVER_NAME

#define RPL_WELCOME                 1
#define RPL_WELCOME_T               " :Welcome to the Internet Relay Network\r\n"
// :server 001 <nick> :Welcome to the Internet Relay Network

#define ERR_NONICKNAMEGIVEN         431
#define ERR_NONICKNAMEGIVEN_T       " :No nickname given\r\n"
// :server 431 <nick> :No nickname given

#define ERR_ERRONEUSNICKNAME        432
#define ERR_ERRONEUSNICKNAME_T      " :Erroneous nickname\r\n"
//:server 432 <user> <nickname> :Erroneous nickname

#define ERR_NOTONCHANNEL            442
#define ERR_NOTONCHANNEL_T          " :You're not on that channel\r\n"
//:server 442 <user> <channel> :You're not on that channel

#define ERR_NOTREGISTERED           451
#define ERR_NOTREGISTERED_T         " :You have not registered\r\n"
// :server 451 <nick> :You have not registered TODO:

#define ERR_NEEDMOREPARAMS          461
#define ERR_NEEDMOREPARAMS_T        " :Not enough parameters\r\n"
// :server 461 <nick> <command> :Not enough parameters

#define ERR_ALREADYREGISTERED       462
#define ERR_ALREADYREGISTERED_T     " :You may not reregister\r\n"
// :server 462 <nick> :You may not reregister TODO:

#define ERR_PASSWDMISMATCH          464
#define ERR_PASSWDMISMATCH_T        " :Password incorrect..\r\n"
// :server 464 <nick> :Password incorrect.. TODO:

#define ERR_KEYSET                  467
#define ERR_KEYSET_T                " :Channel key already set\r\n"
//:server 467 <user> <channel> :Channel key already set

#define ERR_CHANNELISFULL           471
#define ERR_CHANNELISFULL_T         " :Cannot join channel (+l)\r\n"
// :server 471 <nick> <channel> :Cannot join channel (+l)

#define ERR_UNKNOWNMODE             472
#define ERR_UNKNOWNMODE_T           " :is unknown mode char to me\r\n"
//:server 472 <user> <char> :is unknown mode char to me

#define ERR_INVITEONLYCHAN          473
#define ERR_INVITEONLYCHAN_T        " :Cannot join channel (+i)\r\n"
// :server 473 <nick> <channel> :Cannot join channel (+i)

#define ERR_BADCHANNELKEY           475
#define ERR_BADCHANNELKEY_T         " :Cannot join channel (+k)\r\n"
// :server 475 <nick> <channel> :Cannot join channel (+k)

#define ERR_CHANOPRIVSNEEDED        482
#define ERR_CHANOPRIVSNEEDED_T      " :You're not channel operator\r\n"
// :server 482 <nick> <channel> :You're not channel operator

#define ERR_NOSUCHNICK              401
#define ERR_NOSUCHNICK_T            " :No such nick/channel\r\n"
// :server 401 <nick> <target_nick> :No such nick/channel

#define ERR_NOSUCHCHANNEL           403
#define ERR_NOSUCHCHANNEL_T         " :No such channel\r\n"
// :server 403 <nick> <channel> :No such channel

#define ERR_UNKNOWNCOMMAND          421
#define ERR_UNKNOWNCOMMAND_T        " :Unknown command\r\n"
// :server 421 <nick> <command> :Unknown command

#define ERR_NICKNAMEINUSE           433
#define ERR_NICKNAMEINUSE_T         " :Nickname is already in use\r\n"
// :server 433 <nick> :Nickname is already in use

#define ERR_USERNOTINCHANNEL        441
#define ERR_USERNOTINCHANNEL_T      " :They aren't on that channel\r\n"
// :server 441 <user> <target_user> <channel> :They aren't on that channel

#define RPL_NOTOPIC                 331
#define RPL_NOTOPIC_T               " :No topic is set\r\n"
// :server 331 <nick> <channel> :No topic is set

#define RPL_NAMREPLY                353
// Custom trailing message
// :server 353 <nick> <channel> :<names>

#define RPL_ENDOFNAMES              366
#define RPL_ENDOFNAMES_T            " :End of /NAMES list\r\n"
// :server 366 <nick> <channel> :End of /NAMES list

#define RPL_CHANNELMODEIS           324
// Custom trailing message
// :server 324 <nick> <channel> :<mode>

#define RPL_TOPIC                   332
// Custom trailing message
// :server 332 <nick> <channel> :<topic>

#define RAW_MODE                    1000
// :server MODE #channel +m

#define RAW_JOIN                    1001
// :username JOIN #channel

#define RAW_PART                    1002
// :username PART <channel> [<message>]

#define RAW_QUIT                    1003
// :username QUIT [<message>]