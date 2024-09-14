#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"

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

Client::Client(
    const std::string& nickName,
    const std::string& userName,
    const std::string& fullName,
    int fd,
    ARole* role)
    :   _has_set_password(false),
        _nickName(nickName),
        _userName(userName),
        _fullName(fullName),
        _fd(fd),
        _role(role)
        
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

void Client::execute_command(const std::string &message)
{
    std::vector<t_IRCCommand> parsed_commands = parse_client_commands(message);

    for (size_t i = 0; i < parsed_commands.size(); i++) {
        std::map<std::string, CommandFunction>::iterator it = _commandMap.find(parsed_commands[i].command);
        if (it != _commandMap.end()) {
            (this->*(it->second))(parsed_commands[i]);
        } else {
            // TODO: executeUnknown
        }
    }
}

// INIT MAP TO STORE ALL COMMANDS
void Client::initializeCommandMap() {
    _commandMap["KICK"]  = &Client::executeKick;
    _commandMap["INVITE"] = &Client::executeInvite;
    _commandMap["TOPIC"] = &Client::executeTopic;
    _commandMap["MODE"] = &Client::executeMode;
    _commandMap["PASS"] = &Client::executePass;
    _commandMap["NICK"] = &Client::executeNick;
    _commandMap["USER"] = &Client::executeUser;
}

// AUTHENTICATION

bool Client::is_authenticated()
{
    return _has_set_password && !_nickName.empty() && !_userName.empty() && !_fullName.empty();
}