#include "../../include/client/OperatorRole.hpp"

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

void    OperatorRole::mode(char arg)
{
    (void)arg;
}