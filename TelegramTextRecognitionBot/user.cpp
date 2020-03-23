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

QString User::toString()
{
    QString user_info;
    user_info += "User id = " + QString::number(id) + " and username is : " + username;
    return user_info;
}
