#include "../../include/server/RegularRole.hpp"

#include "../../include/server/Socket.hpp"

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

void    RegularRole::kick()
{
    // send permission error
}

void    RegularRole::invite()
{
    // send permission error
}

void    RegularRole::topic()
{
    // send permission error
}

void    RegularRole::changeMode(char arg)
{
    (void)arg;
    // send permission error
}