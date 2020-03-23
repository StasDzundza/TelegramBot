#include "bot.h"
#include <iostream>
#include <QJsonDocument>
#include "telegramtypesfactory.h"
Bot::Bot()
{
    update_access_manager = new QNetworkAccessManager(this);
    update_request = new QNetworkRequest();
    connect(update_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveUpdates(QNetworkReply*)));
}

void Bot::start()
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    update_request->setSslConfiguration(sslConfiguration);
    update_request->setUrl(QUrl(telegram_api_url+bot_token+"/getUpdates"));
    update_access_manager->get(*update_request);
}

void Bot::receiveUpdates(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QByteArray telegram_answer = reply->readAll();
        //std::cout << telegram_answer.toStdString() << "\n\n";
        QJsonDocument json_document = QJsonDocument::fromJson(telegram_answer);
        QJsonObject rootObject = json_document.object();
        QVector<Update*> updates = TelegramTypesFactory::parseUpdates(rootObject);
        int newest_update_id = -1;
        foreach(Update *update,updates){
            std::cout << update->getMessage()->getText().toStdString()<<std::endl;
            if(update->getUpdateId() > newest_update_id){
                newest_update_id = update->getUpdateId();
            }
            sendTextMessageToUser(QString::number(update->getMessage()->getUser()->getId()),update->toString());
            delete update;
        }
        update_request->setUrl(QUrl(telegram_api_url+bot_token+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        update_access_manager->get(*update_request);
    }
}

void Bot::sendTextMessageToUser(const QString&user_id,const QString&message)
{
    QNetworkRequest request;
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setSslConfiguration(sslConfiguration);
    request.setUrl(QUrl(telegram_api_url+bot_token+"/sendMessage?chat_id=" + user_id + "&text=" + message));
    update_access_manager->get(request);
}
