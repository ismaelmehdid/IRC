#include "../../include/irc.hpp"
#include "../../include/client/Client.hpp"
#include "../../include/client/ARole.hpp"
#include "../../include/server/Server.hpp"

ARole::ARole() : _client(NULL) { }

ARole::ARole(Client *client) : _client(client) { }

ARole::~ARole() { }

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

}

void ARole::user(const t_IRCCommand &command)
{
    if (!_client->is_authenticated() || !_client.) {
        if (command.params.size() < 3) {
            global_ircserv->_socket.send(_client->get_fd(), ERR_NEED_MORE_PARAMS);
        } else if () {
            return ;
        }
    } else {
        global_ircserv->_socket.send(_client->get_fd(), ERR_ALREADY_REGISTERED);
    }
}
