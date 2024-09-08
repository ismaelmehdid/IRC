#include "../../include/server/OperatorRole.hpp"

OperatorRole::OperatorRole() {}

OperatorRole::OperatorRole(const OperatorRole& other)
{
    (void)other; // nothing to copy
}

OperatorRole&   OperatorRole::operator=(const OperatorRole& other)
{
    (void)other; // nothing to copy
    return (*this);
}

OperatorRole::~OperatorRole() {}

void    OperatorRole::kick()
{
}

void    OperatorRole::invite()
{
}

void    OperatorRole::topic()
{
}

void    OperatorRole::changeMode(char arg)
{
    (void)arg;
}