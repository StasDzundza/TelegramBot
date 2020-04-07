#include <iostream>
#include <QJsonDocument>
#include <QTextStream>
#include <sstream>
#include <QPointer>
#include "bot.h"
#include "telegramtypesfactory.h"
#include "textreader.h"

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
    update_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/getUpdates"));
    update_access_manager.get(update_request);
}

void Bot::receiveUpdates(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QByteArray telegram_answer = reply->readAll();
        //std::cout << telegram_answer.toStdString() << std::endl;
        QJsonDocument json_document = QJsonDocument::fromJson(telegram_answer);
        QJsonObject rootObject = json_document.object();
        QVector<Update*> updates = TelegramTypesFactory::parseUpdates(rootObject);
        int newest_update_id = -1;
        foreach(Update *update,updates){
            std::cout << update->getMessage()->getText().toStdString() << std::endl;
            if(update->getUpdateId() > newest_update_id){
                newest_update_id = update->getUpdateId();
            }
            processUpdate(update);
            delete update;
        }
        update_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        update_access_manager.get(update_request);
    }
}

void Bot::processUpdate(const Update *update)
{
    if(!update->getMessage()->getText().isEmpty()){
        QVector<QString>commands = TextReader::splitTextByWords(update->getMessage()->getText());
        if(commands.at(0) == "/translate_text" and commands.size() >= 4){
            QString text_to_translate = TextReader::getTextAfterNthWord(update->getMessage()->getText(),3);
            QPointer<Translater> translater = new Translater(this,update->getMessage()->getUser()->getId());
            translater->translateText(text_to_translate,commands.at(1),commands.at(2));
        }else{
            sendGetFileRequest(update->getMessage()->getPhotoId());
            sendTextMessageToUser(QString::number(update->getMessage()->getUser()->getId()),update->toString() + INVALID_COMMAND);
        }
    }
}

void Bot::receiveTranslatedText(const QString &translated_text, int user_id)
{
    if(translated_text.isEmpty()){
        sendTextMessageToUser(QString::number(user_id),INVALID_COMMAND);
    }
    else {
        sendTextMessageToUser(QString::number(user_id),translated_text);
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
        std::cout << TELEGRAM_API_URL.toStdString() <<"file/bot"<< BOT_TOKEN.toStdString() << '/' << file->getFilePath().toStdString() << std::endl;
    }
}

void Bot::sendTextMessageToUser(const QString&user_id,const QString&message)
{
    send_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/sendMessage?chat_id=" + user_id + "&text=" + message));
    send_access_manager.get(send_request);
}

void Bot::sendGetFileRequest(const QString &file_id)
{
    file_request.setUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/getFile?file_id="+file_id);
    file_access_manager.get(file_request);
}
