#include "user.h"

User::User(){

}

int User::getId() const
{
    return id;
}

void User::setId(int value)
{
    id = value;
}

QString User::getUsername() const
{
    return username;
}

void User::setUsername(const QString &value)
{
    username = value;
}
