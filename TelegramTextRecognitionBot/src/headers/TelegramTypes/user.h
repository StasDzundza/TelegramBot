#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    int getId() const;
    void setId(int value);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString toString();

private:
    int id = -1;
    QString username;
};

#endif // USER_H
