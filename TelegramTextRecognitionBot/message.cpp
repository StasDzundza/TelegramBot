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

void Message::setMessageId(int value)
{
    message_id = value;
}

QString Message::getText() const
{
    return text;
}

void Message::setText(const QString &value)
{
    text = value;
}

QVector<QString>& Message::getPhotosId()
{
    return photos_id;
}

void Message::setPhotosId(const QVector<QString> &value)
{
    photos_id = value;
}

void Message::addPhotoId(const QString &photo_id)
{
    photos_id.push_back(photo_id);
}


