#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"
#include "../../include/client/ARole.hpp"
#include "../../include/server/Server.hpp"

ARole::ARole() : _client(NULL) { }

ARole::ARole(Client *client) : _client(client) { }

ARole::~ARole() { }

/**
 * @brief Passes a command to the ARole object.
 *
 * This function is responsible for handling the pass command received by the ARole object.
 * It checks if the client is authenticated and performs the necessary actions based on the command parameters.
 * If the client is not authenticated, it checks if the command parameters are empty or if the password is incorrect.
 * If the password is correct, it sets the _has_set_password flag to true.
 * If the client is already registered, it sends an error message to the client.
 *
 * @param command The IRC command to be passed.
 */
void ARole::pass(const t_IRCCommand &command)
{
    if (!_client->is_authenticated()) {
        if (command.params.empty()) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        } else if (command.params[0] != global_ircserv->get_password()) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        } else {
            _client->_has_set_password = true;
        }
    } else {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

void ARole::nick(const t_IRCCommand &command)
{
    if (!_client->is_authenticated()) {
        if (!_client->_has_set_password) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        }  else if (command.params.empty()) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        } else {
            std::string newNick = command.params[0];
            if (global_ircserv->isNickNameTaken(newNick)) {
                global_ircserv->_socket.send(_client->get_fd(), ERR_NICKNAME_IN_USE);
            } else {
                _client->setNickName(newNick);
            }
        }
    } else {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}

void ARole::user(const t_IRCCommand &command)
{
    if (!_client->is_authenticated() && _client->getUserName().empty() && _client->getFullName().empty()) {
        if (!_client->_has_set_password) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_PASSWORD_INCORRECT);
        } else if (_client->getNickName().empty()) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        } else if (command.params.size() < 3 || command.trailing.empty()) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        } else {
            _client->setUserName(command.params[0]);
            _client->setFullName(command.trailing);
            std::cout << GREEN << "Client connected!" << RESET << std::endl;
            global_ircserv->_socket.send(_client->get_fd(), MSG_WELCOME);
        }
    } else {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}
