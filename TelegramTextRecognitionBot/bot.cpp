#include "bot.h"
#include <iostream>
#include <QJsonDocument>
#include "telegramtypesfactory.h"

Bot::Bot()
{

    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());

    update_request.setSslConfiguration(sslConfiguration);
    send_request.setSslConfiguration(sslConfiguration);
    file_request.setSslConfiguration(sslConfiguration);

    connect(&update_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveUpdates(QNetworkReply*)));
    connect(&send_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(sendResultHandler(QNetworkReply*)));
    connect(&file_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveFile(QNetworkReply*)));
}

void Bot::start()
{
    update_request.setUrl(QUrl(telegram_api_url+"bot"+bot_token+"/getUpdates"));
    update_access_manager.get(update_request);
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
            sendGetFileRequest(update->getMessage()->getPhotoId());
            sendTextMessageToUser(QString::number(update->getMessage()->getUser()->getId()),update->toString());
            delete update;
        }
        update_request.setUrl(QUrl(telegram_api_url+"bot"+bot_token+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        update_access_manager.get(update_request);
    }
}

void Bot::sendResultHandler(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        std::cout << "send Success" << std::endl;
    }else{
        std::cout << "send Error" << std::endl;
    }
}

void Bot::receiveFile(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QByteArray telegram_answer = reply->readAll();
        QJsonDocument json_document = QJsonDocument::fromJson(telegram_answer);
        QJsonObject rootObject = json_document.object();
        File *file = TelegramTypesFactory::createFile(rootObject);
        std::cout << telegram_api_url.toStdString() <<"file/bot"<< bot_token.toStdString() << '/' << file->getFilePath().toStdString() << std::endl;
    }
}

void Bot::sendTextMessageToUser(const QString&user_id,const QString&message)
{
    send_request.setUrl(QUrl(telegram_api_url+"bot"+bot_token+"/sendMessage?chat_id=" + user_id + "&text=" + message));
    send_access_manager.get(send_request);
}

void Bot::sendGetFileRequest(const QString &file_id)
{
    file_request.setUrl(telegram_api_url+"bot"+bot_token+"/getFile?file_id="+file_id);
    file_access_manager.get(file_request);
}
