#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMap>
#include <set>
#include "translater.h"
#include "update.h"

class Bot : public QObject
{
    Q_OBJECT
public:
    Bot();
    void start();

public slots:
    void receiveTranslatedText(const QString&translated_text,int user_id);

private slots:
    void receiveUpdates(QNetworkReply*reply);

    void sendResultHandler(QNetworkReply*reply);

    void receiveFile(QNetworkReply*reply);

private:
    const QString BOT_TOKEN = "1055976603:AAHeHiEW4QLNKqGduZazziVToKlRe_gSN-8";
    const QString TELEGRAM_API_URL = "https://api.telegram.org/";
    const QString INVALID_COMMAND = "INVALID COMMAND. Write /commands and check valid and existing commands.";
    const std::set<QString> VALID_COMMANDS{"/translate_text","/translate_file"};
    QMap<int,QString> last_user_commands;

    QNetworkAccessManager update_access_manager;
    QNetworkRequest update_request;

    QNetworkAccessManager send_access_manager;
    QNetworkRequest send_request;

    QNetworkAccessManager file_access_manager;
    QNetworkRequest file_request;

    void processUpdate(const Update*update);

    void executeUserCommand(const Update*update);

    void sendTextMessageToUser(const QString& chat_id,const QString& message);

    void sendReplyToUserCommand(const Update*update);

    void sendGetFileRequest(const QString& file_id);

};

#endif // BOT_H
