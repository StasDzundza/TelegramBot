#ifndef TELEGRAMTYPESFACTORY_H
#define TELEGRAMTYPESFACTORY_H

#include "update.h"
#include "QJsonObject"

class TelegramTypesFactory
{
public:
    TelegramTypesFactory();

    static QVector<Update *> parseUpdates(const QJsonObject& updates);

    static Update *createUpdate(const QJsonObject&update_json_object);

    static Message *createMessage(const QJsonObject&message_json_object);

    static User *createUser(const QJsonObject&user_json_object);
};

#endif // TELEGRAMTYPESFACTORY_H
