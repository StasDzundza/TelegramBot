#include "message.h"

Message::Message()
{

}

void Message::setUser(const User &user){
    this->user = std::make_shared<User>(user);
}

User *Message::getUser()const{
    return user.get();
}

void Message::setDocument(const Document &document)
{
    this->document = std::make_shared<Document>(document);
}

Document *Message::getDocument()const
{
    return document.get();
}

void Message::setPhotoId(const QString &photo_id)
{
    this->photo_id = photo_id;
}

QString Message::getPhotoId()
{
    return photo_id;
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

QString Message::toString()
{
    QString message_info;
    message_info += "Message id = " + QString::number(message_id) + '\n';
    message_info += "Message text : " + text + '\n';
    message_info += "User info : " + user->toString();
    return message_info;
}

QString Message::getCaption() const
{
    return caption;
}

void Message::setCaption(const QString &value)
{
    caption = value;
}

QString Message::getFilename() const
{
    return filename;
}

void Message::setFilename(const QString &value)
{
    filename = value;
}

bool Message::isEmpty()
{
    return text.isEmpty() && document->isEmpty() && photo_id.isEmpty();
}


