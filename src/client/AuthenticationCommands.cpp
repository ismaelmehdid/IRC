#include "../../include/server/Server.hpp"

void    ARole::cap(const t_IRCCommand &command)
{
	if (command.params[0] == "LS")
	{
         // server doesn't have any special features
        std::string empty_list_response = ":server CAP " + _client->getNickName() + "LS :\r\n";
        global_ircserv->socketSend(_client->get_fd(), empty_list_response);
	}
}

void    ARole::pass(const t_IRCCommand &command)
{
    if (!_client->is_authenticated())
    {
        if (command.params.empty())
        {
            global_ircserv->socketSend(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else if (command.params[0] != global_ircserv->get_password())
        {
            global_ircserv->socketSend(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        }
        else
        {
            _client->_has_set_password = true;
            //TODO: Notify that the client have put the right password
        }
    }
    else
    {
        global_ircserv->socketSend(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

/**
 * @brief Changes the nickname of the client.
 * 
 * This function is used to change the nickname of the client. If the client is not authenticated,
 * it checks if a password has been set. If a password has not been set, it sends an error message
 * indicating that the password is incorrect. If the command parameters are empty, it sends an error
 * message indicating that more parameters are needed. If the nickname is already taken by another
 * client, it sends an error message indicating that the nickname is in use. Otherwise, it sets the
 * new nickname for the client.
 * 
 * If the client is already authenticated, it sends an error message indicating that the client is
 * already registered.
 * 
 * @param command The IRC command containing the new nickname.
 */

static bool is_nickname_valid(const std::string &nick)
{
    if (std::isdigit(nick[0]) || nick[0] == '#' || nick[0] == '&')
        return (false);
    for (size_t i = 1; i < nick.size(); i++)
    {
        if (!std::isalnum(nick[0]) &&
            nick[0] != '-' &&
            nick[0] != '[' &&
            nick[0] != ']' &&
            nick[0] != '\\' &&
            nick[0] != '^' &&
            nick[0] != '_' &&
            nick[0] != '{' &&
            nick[0] != '}'
            )
            return (false);
    }
    return (true);
}

void    ARole::nick(const t_IRCCommand &command)
{
    if (!_client->_has_set_password)
    {
        global_ircserv->socketSend(_client->get_fd(), ERR_PASSWORD_REQUIRED);
    } 
    else if (command.params.empty())
    {
        global_ircserv->socketSend(_client->get_fd(), ERR_NEED_MORE_PARAMS);
    }
    else
    {
        std::string newNick = command.params[0];
        if (_client->getNickName() == newNick)
            return ;
        if (!is_nickname_valid(newNick))
        {
            global_ircserv->socketSend(_client->get_fd(), ":server 432 * " + newNick + " :Erroneous nickname\r\n");
        }
        else if (global_ircserv->isNickNameTaken(newNick))
        {
            global_ircserv->socketSend(_client->get_fd(), ERR_NICKNAME_IN_USE);
        }
        else
        {
            // TODO: broadcast new nickname to all users in channel
            _client->setNickName(newNick);
        }
    }
}

/**
 * @brief Handles the user command received from the client.
 * 
 * This function is responsible for processing the user command received from the client.
 * It checks if the client is authenticated and if the necessary parameters are provided.
 * If the client is not authenticated and the required parameters are provided, it sets the client's username and full name,
 * sends a welcome message to the client, and marks the client as registered.
 * If the client is already registered, it sends an error message indicating that the client is already registered.
 * 
 * @param command The IRC command received from the client.
 */
void    ARole::user(const t_IRCCommand &command)
{
    if (!_client->is_authenticated() && _client->getUserName().empty() && _client->getFullName().empty())
    {
        if (!_client->_has_set_password)
        {
            global_ircserv->socketSend(_client->get_fd(), ERR_PASSWORD_REQUIRED);
        }
        else if (_client->getNickName().empty())
        {
            global_ircserv->socketSend(_client->get_fd(), ERR_NONICKNAMEGIVEN);
        }
        else if (command.params.size() < 3 || command.trailing.empty())
        {
            global_ircserv->socketSend(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        }
        else
        {
            _client->setUserName(command.params[0]);
            _client->setFullName(command.trailing);
            std::cout << GREEN << "Client connected!" << RESET << std::endl;

            global_ircserv->socketSend(_client->get_fd(),
                _client->getPrefix() + WELCOME_CODE + _client->getNickName() + MSG_WELCOME);

        }
    }
    else
    {
        global_ircserv->socketSend(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}