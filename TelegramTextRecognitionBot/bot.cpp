#include "bot.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
        QJsonDocument json_document = QJsonDocument::fromJson(telegram_answer);
        QJsonObject rootObject = json_document.object();
        QJsonArray result = rootObject["result"].toArray();
        int newest_update_id = 0;
        foreach(auto&& json_value,result){
            QJsonObject json_object = json_value.toObject();
            int update_id = json_object["update_id"].toInt();
            if(update_id>newest_update_id){
                newest_update_id = update_id;
            }
            QJsonObject message = json_object["message"].toObject();
            QString text = message["text"].toString();
            QJsonObject user = message["from"].toObject();
            QString username = user["username"].toString();
            std::cout << "Received message \"" <<text.toStdString()<< "\" "<<  "from : @" << username.toStdString() << std::endl;

        }
        request->setUrl(QUrl(telegram_api_url+bot_token+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        access_manager->get(*request);
    }
}
