#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"
#include "../../include/server/Server.hpp"

Client::Client()
    :   _has_set_password(false),
        _nickName(),
        _userName(),
        _fullName(),
        _fd(-1),
        _role(new RegularRole(this))
{
    initializeCommandMap();
}

Client::Client(int fd)
    :   _has_set_password(false),
        _nickName(),
        _userName(),
        _fullName(),
        _fd(fd),
        _role(new RegularRole(this))
        
{
    initializeCommandMap();                
}

Client::Client (const Client& other)
    :   _has_set_password(other._has_set_password),
        _nickName(other._nickName),
        _userName(other._userName),
        _fullName(other._fullName),
        _fd(other.get_fd()),
        _role(other._role ? other._role->clone() : NULL)
{
    initializeCommandMap();
}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        delete (this->_role);
        close (this->_fd);
        this->_fd = other.get_fd();
        this->_role = other._role ? other._role->clone() : NULL;
        this->_nickName = other._nickName;
        this->_userName = other._userName;
        this->_fullName = other._fullName;
        this->_has_set_password = other._has_set_password;
    }
    return (*this);
}

Client::~Client()
{
    std::cout << "Client destroyed." << std::endl;
    delete (this->_role);
    close (this->_fd);
}

int Client::get_fd() const
{
    return (this->_fd);
}

// COMMANDS
void Client::executeKick(const t_IRCCommand &command)
{
    this->_role->kick(command);
}

void Client::executeInvite(const t_IRCCommand &command)
{
    this->_role->invite(command);
}

void Client::executeTopic(const t_IRCCommand &command)
{
    this->_role->topic(command);
}

void Client::executeMode(const t_IRCCommand &command)
{
    this->_role->mode(command);
}

void Client::executePass(const t_IRCCommand &command)
{
    this->_role->pass(command);
}

void Client::executeNick(const t_IRCCommand &command)
{
    this->_role->nick(command);
}

void Client::executeUser(const t_IRCCommand &command)
{
    this->_role->user(command);
}

void Client::setRole(ARole* newRole)
{
    delete (this->_role);
    this->_role = newRole;
}

/**
 * Executes the given client command.
 *
 * This function parses the provided message into a vector of IRC commands and iterates over each command.
 * For each command, it looks up the corresponding command function in the _commandMap and executes it.
 * If the command is not found in the map, it sends the ERR_UNKNOWNCOMMAND response to the client.
 *
 * @param message The message containing the client command.
 */
void Client::execute_command(const std::string &message)
{
    std::vector<t_IRCCommand> parsed_commands = parse_client_commands(message);

    for (size_t i = 0; i < parsed_commands.size(); i++) {
        std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
        if (it != _commandMap.end()) {
            (this->*(it->second))(parsed_commands[i]);
        } else {
            global_ircserv->_socket.send(_fd, ERR_UNKNOWNCOMMAND);
        }
    }
}

/**
 * @brief Initializes the command map for the client.
 * 
 * This function assigns the appropriate member functions of the Client class to the corresponding commands in the command map.
 * The command map is a std::map that maps command names to member function pointers.
 * 
 * The following commands are supported:
 * - KICK: Executes the kick command.
 * - INVITE: Executes the invite command.
 * - TOPIC: Executes the topic command.
 * - MODE: Executes the mode command.
 * - PASS: Executes the pass command.
 * - NICK: Executes the nick command.
 * - USER: Executes the user command.
 */
void Client::initializeCommandMap() {
    _commandMap["KICK"]  = &Client::executeKick;
    _commandMap["INVITE"] = &Client::executeInvite;
    _commandMap["TOPIC"] = &Client::executeTopic;
    _commandMap["MODE"] = &Client::executeMode;
    _commandMap["PASS"] = &Client::executePass;
    _commandMap["NICK"] = &Client::executeNick;
    _commandMap["USER"] = &Client::executeUser;
}

/**
 * @brief Checks if the client is authenticated.
 * 
 * @return true if the client is authenticated, false otherwise.
 */
bool Client::is_authenticated()
{
    return _has_set_password && !_nickName.empty() && !_userName.empty() && !_fullName.empty();
}

//Getters
std::string Client::getNickName()
{
    return _nickName;
}

std::string Client::getUserName()
{
    return _userName;
}

std::string Client::getFullName()
{
    return _fullName;
}

//Setters
void    Client::setNickName(const std::string &nickName)
{
    _nickName = nickName;
}

void    Client::setUserName(const std::string &userName)
{
    _userName = userName;
}

void    Client::setFullName(const std::string &fullName)
{
    _fullName = fullName;
}
