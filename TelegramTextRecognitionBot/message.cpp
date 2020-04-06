#include "message.h"
#include <memory>
Message::Message()
{

}

void Message::setUser(const User &user){
    this->user = std::make_shared<User>(user);
}

User *Message::getUser(){
    return user.get();
}

int Message::getMessageId() const
{
    return message_id;
}

void Message::setMessageId(int message_id)
{
    this->message_id = message_id;
}

QString Message::getText() const
{
    return text;
}

void Message::setText(const QString &text)
{
    this->text = text;
}

QString& Message::getPhotoId()
{
    return photo_id;
}

void Message::setPhotoId(const QString &photo_id)
{
    this->photo_id = photo_id;
}

QString Message::toString()
{
    QString message_info;
    message_info += "Message id = " + QString::number(message_id) + '\n';
    message_info += "Message text : " + text + '\n';
    message_info += "User info : " + user->toString();
    return message_info;
}


