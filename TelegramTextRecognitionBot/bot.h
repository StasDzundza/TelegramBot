#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVector>
#include "translater.h"

class Bot : public QObject
{
    Q_OBJECT
public:
    Bot();
    void start();

public slots:
    void receiveTranslatedText(const QString&translated_text,int user_id);

signals:

private slots:
    void receiveUpdates(QNetworkReply*reply);

    void sendResultHandler(QNetworkReply*reply);

    void receiveFile(QNetworkReply*reply);

private:
    const QString bot_token = "1055976603:AAHeHiEW4QLNKqGduZazziVToKlRe_gSN-8";
    const QString telegram_api_url = "https://api.telegram.org/";

    QNetworkAccessManager update_access_manager;
    QNetworkRequest update_request;

    QNetworkAccessManager send_access_manager;
    QNetworkRequest send_request;

    QNetworkAccessManager file_access_manager;
    QNetworkRequest file_request;

    void parseUserRequest(QVector<QString>& commands,const QString& text);

    void sendTextMessageToUser(const QString& chat_id,const QString& message);

    void sendGetFileRequest(const QString& file_id);
};

#endif // BOT_H
