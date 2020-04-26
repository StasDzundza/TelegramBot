#ifndef MESSAGE_H
#define MESSAGE_H

#include <QImage>
#include <QVector>
#include <memory>
#include "user.h"
#include "document.h"

class Message
{
public:
    Message();

    void setUser(const User&user);
    User* getUser()const;

    void setDocument(const Document&document);
    Document *getDocument()const;

    void setPhotoId(const QString& photo_id);
    QString getPhotoId();

    int getMessageId() const;
    void setMessageId(int value);

    QString getText() const;
    void setText(const QString &value);

    QString toString();

    QString getCaption() const;
    void setCaption(const QString &value);

    QString getFilename() const;
    void setFilename(const QString &value);

    bool isEmpty();

private:
    int message_id = -1;
    std::shared_ptr<User> user;
    std::shared_ptr<Document> document;
    QString photo_id;
    QString text;
    QString caption;
    QString filename;
};

#endif // MESSAGE_H
