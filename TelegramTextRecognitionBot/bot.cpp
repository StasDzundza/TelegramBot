#include "bot.h"
#include <iostream>
#include <QJsonDocument>
#include "telegramtypesfactory.h"
Bot::Bot()
{
    access_manager = new QNetworkAccessManager(this);
    request = new QNetworkRequest();
    connect(access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveUpdates(QNetworkReply*)));
}

void Bot::start()
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    request->setSslConfiguration(sslConfiguration);
    request->setUrl(QUrl(telegram_api_url+bot_token+"/getUpdates"));
    access_manager->get(*request);
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
        }
        request->setUrl(QUrl(telegram_api_url+bot_token+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        access_manager->get(*request);
    }
}
