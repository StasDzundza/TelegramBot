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
        std::cout << telegram_answer.toStdString() << std::endl;
        QJsonDocument json_document = QJsonDocument::fromJson(telegram_answer);
        QJsonObject rootObject = json_document.object();
        QVector<Update*> updates = TelegramTypesFactory::parseUpdates(rootObject);
        int newest_update_id = -1;
        foreach(Update *update,updates){
            if(update->getUpdateId() > newest_update_id){
                newest_update_id = update->getUpdateId();
            }
            //processUpdate(update);
            sendTextMessageToUser(QString::number(update->getMessage()->getUser()->getId()),QString::fromStdString(telegram_answer.toStdString()));
            delete update;
        }
        update_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        update_access_manager.get(update_request);
    }
}

void Bot::processUpdate(const Update *update)
{
    if(!update->getMessage()->getText().isEmpty()){
        int user_id = update->getMessage()->getUser()->getId();
        if((VALID_COMMANDS.find(update->getMessage()->getText()) != VALID_COMMANDS.end()) ||
                (last_user_commands.contains(user_id) && !last_user_commands[user_id].isEmpty())){//check if commannd is valid
            if(last_user_commands.contains(user_id)){
                if(!last_user_commands[user_id].isEmpty()){
                    executeUserCommand(update);
                    last_user_commands[user_id].clear();
                }else{
                    last_user_commands[user_id] = update->getMessage()->getText();
                    sendReplyToUserCommand(update);
                }
            }else{
                last_user_commands[user_id] = update->getMessage()->getText();
                sendReplyToUserCommand(update);
            }
        }else{
            sendTextMessageToUser(QString::number(update->getMessage()->getUser()->getId()),INVALID_COMMAND);
        }
    }
}

void Bot::executeUserCommand(const Update *update)
{
    int user_id = update->getMessage()->getUser()->getId();
    QString last_user_command = last_user_commands[user_id];
    if(last_user_command == "/translate_text"){
        QVector<QString>languages = TextReader::getFirstNWords(update->getMessage()->getText(),2);
        QString text_to_translate = TextReader::getTextAfterNthWord(update->getMessage()->getText(),2);
        if(!text_to_translate.isEmpty() and languages.size() == 2){
            QPointer<Translater> translater = new Translater(this,user_id);
            translater->translateText(text_to_translate,languages.at(0),languages.at(1));
        }else{
            sendTextMessageToUser(QString::number(user_id),update->toString() + INVALID_COMMAND);
        }
    }else if(last_user_command == "/translate_file"){
        QPointer<TelegramFileDownloader> file_downloader = new TelegramFileDownloader(this,user_id);
        file_downloader->downloadFile(update->getMessage()->getDocument()->getFileId());
    }

}

void Bot::sendReplyToUserCommand(const Update *update)
{
    int user_id = update->getMessage()->getUser()->getId();
    QString last_user_command = last_user_commands[user_id];
    QString reply;
    if(last_user_command == "/translate_text"){
        reply = "Send text to the bot in format : <source_lang> <target_lang> <some text>.\nSource and target languages should be in 2-letter format. "
                "For example : English - en, Russian - ru.\nAlso bot can detect source language. For that write to <souce_lang> auto.";
    }else if(last_user_command == "/translate_file"){
        reply = "Send text file to the bot with capture : <source_lang> <target_lang>.\nSource and target languages should be in 2-letter format."
                "For example : English - en, Russian - ru.\nAlso bot can detect source language. For that write to <souce_lang> auto.";
    }
    sendTextMessageToUser(QString::number(user_id),reply);
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

void Bot::receiveLocalFilePath(const QString &local_file_path, int user_id)
{
    if(!local_file_path.isEmpty()){
        sendTextMessageToUser(QString::number(user_id),local_file_path);
    }else{
        sendTextMessageToUser(QString::number(user_id),"File receiving error! Try again.");
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
