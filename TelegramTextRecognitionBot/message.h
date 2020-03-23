#ifndef MESSAGE_H
#define MESSAGE_H

#include <QImage>
#include <QVector>
#include <memory>
#include "user.h"
class Message
{
public:
    Message();
    void setUser(const User&user);
    User* getUser();

    int getMessageId() const;
    void setMessageId(int value);

    QString getText() const;
    void setText(const QString &value);

    QString& getPhotoId();
    void setPhotoId(const QString &value);

    QString toString();

private:
    int message_id = -1;
    std::shared_ptr<User> user;
    QString text;
    QString photo_id;

};

#endif // MESSAGE_H
