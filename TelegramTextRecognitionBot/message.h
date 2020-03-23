#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
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

    QVector<QString>& getPhotosId();
    void setPhotosId(const QVector<QString> &value);

    void addPhotoId(const QString& photo_id);

private:
    int message_id = -1;
    std::shared_ptr<User> user;
    QString text;
    QVector<QString> photos_id;

};

#endif // MESSAGE_H
