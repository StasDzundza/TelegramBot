#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Bot : public QObject
{
    Q_OBJECT
public:
    Bot();
    void start();
signals:

public slots:

private slots:
    void receiveUpdates(QNetworkReply*reply);
private:
    const QString bot_token = "1055976603:AAHeHiEW4QLNKqGduZazziVToKlRe_gSN-8";
    const QString telegram_api_url = "https://api.telegram.org/bot";

    QNetworkAccessManager *access_manager;
    QNetworkRequest *request;
};

#endif // BOT_H
