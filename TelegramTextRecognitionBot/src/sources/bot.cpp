#include <iostream>
#include <QJsonDocument>
#include <QPointer>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include "bot.h"
#include "telegramtypesfactory.h"
#include "textreader.h"
#include "translater.h"
#include "filedownloader.h"
#include "tesseractocr.h"
#include "filewriter.h"

Bot::Bot()
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    update_request.setSslConfiguration(sslConfiguration);
    send_request.setSslConfiguration(sslConfiguration);
    connect(&update_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveUpdates(QNetworkReply*)));
    connect(&send_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveSendingResult(QNetworkReply*)));
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
            processUpdate(update);
        }
        update_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/getUpdates?offset=" + QString::number(newest_update_id+1)));
        update_access_manager.get(update_request);
    }
}

void Bot::processUpdate(Update *update)
{
    if(!update->getMessage()->isEmpty()){
        int user_id = update->getMessage()->getUser()->getId();
        if(VALID_COMMANDS.contains(update->getMessage()->getText()) || //check if commannd is valid
                (last_user_commands.contains(user_id) && !last_user_commands[user_id].isEmpty())){
            if(last_user_commands.contains(user_id) && !last_user_commands[user_id].isEmpty()){
                executeUserCommand(update);
            }else{
                last_user_commands[user_id] = update->getMessage()->getText();
                sendReplyToUserCommand(update);
            }
        }else{
            sendTextMessageToUser(update->getMessage()->getUser()->getId(),INVALID_COMMAND);
        }
    }
}

void Bot::executeUserCommand(Update *update)
{
    int user_id = update->getMessage()->getUser()->getId();
    QString last_user_command = last_user_commands[user_id];
    if(last_user_command == "/start"){
        sendCommandKeyboardToUser(user_id);
    }
    if(last_user_command == "/translate_text"){
        QVector<QString>languages = TextReader::getFirstNWords(update->getMessage()->getText(),2);
        QString text_to_translate = TextReader::getTextAfterNthWord(update->getMessage()->getText(),2);
        if(!text_to_translate.isEmpty() && languages.size() == 2){
            QPointer<Translater> translater = new Translater(this,user_id);
            translater->translateText(text_to_translate,languages.at(0),languages.at(1));
        }else{
            sendTextMessageToUser(user_id,update->toString() + INVALID_COMMAND);
        }
        last_user_commands[user_id].clear();
        delete update;
    }else if(last_user_command == "/translate_file"){
        QString mime_type = update->getMessage()->getDocument()->getMimeType();
        QString image_type = "image";
        auto it = std::search(mime_type.begin(),mime_type.end(),image_type.begin(),image_type.end());
        if(!update->getMessage()->getDocument()->isEmpty() && it == mime_type.end()){
            QPointer<TelegramFileDownloader> file_downloader = new TelegramFileDownloader(this);
            file_downloader->downloadDocument(update);
        }else{
            sendTextMessageToUser(user_id,INVALID_TYPE_OF_TEXT_FILE);
        }
    }else if(last_user_command == "/recognize_photo"){
        if(!update->getMessage()->getPhotoId().isEmpty()){
            QPointer<TelegramFileDownloader> file_downloader = new TelegramFileDownloader(this);
            file_downloader->downloadPhoto(update);
        }else{
            sendTextMessageToUser(user_id,INVALID_TYPE_OF_PHOTO_FILE);
        }
    }else if(last_user_command == "/write_text_to_file"){
        QString text_message = update->getMessage()->getText();
        QString text_uid = QString::number(user_id);
        if(!text_message.isEmpty()){
            QString filename = text_uid + ".txt";
            FileWriter::writeToFile(filename,text_message);
            sendDocumentToUser(user_id,filename);
        }else{
            sendTextMessageToUser(user_id,"Invalid input data!");
        }
        last_user_commands[user_id].clear();
        delete update;
    }
}

void Bot::sendReplyToUserCommand(const Update *update)
{
    int user_id = update->getMessage()->getUser()->getId();
    QString last_user_command = last_user_commands[user_id];
    QString reply;
    if(last_user_command == "/start"){
        sendCommandKeyboardToUser(user_id);
        last_user_commands[user_id].clear();
        return;
    }else if(last_user_command == "/translate_text"){
        reply = "Send text to the bot in format : <source_lang> <target_lang> <some text>.\nSource and target languages should be in 2-letter format. "
                "For example : English - en, Russian - ru.\nAlso bot can detect source language. For that write to <souce_lang> auto.";
    }else if(last_user_command == "/translate_file"){
        reply = "Send text file to the bot with caption : <source_lang> <target_lang>.\nSource and target languages should be in 2-letter format."
                "For example : English - en, Russian - ru.\nAlso bot can detect source language. For that write to <souce_lang> auto.";
    }else if(last_user_command == "/recognize_photo"){
        reply = "Send photo to bot with caption : <source_lang>.\nSource language should be in 3-letter format."
                "For example : English - eng, Russian - rus.";
    }else if(last_user_command == "/write_text_to_file"){
        reply = "Send text message to bot in different language and bot will send file, which contains this text message";
    }
    sendTextMessageToUser(user_id,reply);
}

void Bot::sendCommandKeyboardToUser(int chat_id)
{
    QString reply_markup = "{\"keyboard\": [";
    int i = 0;
    for(auto &command: VALID_COMMANDS){
        reply_markup+="[{\"text\" :\"";
        reply_markup+=command;
        reply_markup+="\"}]";
        if(i == VALID_COMMANDS.size() - 1){
            reply_markup+="]}";
            break;
        }else{
            reply_markup+=",";
        }
        i++;
    }
    send_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/sendMessage?chat_id=" + QString::number(chat_id) +"&text=Choose the command"
    "&reply_markup=" + reply_markup));
    send_access_manager.get(send_request);
}

void Bot::receiveTranslatedText(const QString &translated_text, int user_id)
{
    if(translated_text.isEmpty()){
        sendTextMessageToUser(user_id,"Translation error. Something was wrong %F0%9F%98%93"); // these symbols mean sad emodji
    }
    else {
        sendTextMessageToUser(user_id,translated_text);
        QString text_uid = QString::number(user_id);
        QString filename = text_uid + ".txt";
        FileWriter::writeToFile(filename,translated_text);
        sendDocumentToUser(user_id,filename);
    }
}

void Bot::receiveLocalFilePath(const QString &local_file_path,Update*update)
{
    int user_id = update->getMessage()->getUser()->getId();
    if(!local_file_path.isEmpty()){
        QString last_user_command = last_user_commands[user_id];
        if(last_user_command == "/translate_file"){
            QPointer<Translater> translater = new Translater(this,user_id);
            QString caption = update->getMessage()->getCaption();
            QString langFrom = TextReader::getNthWord(caption,1);
            QString langTo = TextReader::getNthWord(caption,2);
            translater->translateFile(local_file_path,langFrom,langTo);
        }else if(last_user_command == "/recognize_photo"){
            QString caption = update->getMessage()->getCaption();
            QString langFrom = TextReader::getNthWord(caption,1);
            QString text = TesseractOCR::recognizeImage(local_file_path,langFrom);
            sendTextMessageToUser(user_id,text);
        }else{
            sendTextMessageToUser(user_id,INVALID_COMMAND);
        }
        last_user_commands[user_id].clear();
    }else{
        sendTextMessageToUser(user_id,"File receiving error! Try again.");
    }
    delete update;
}

void Bot::receiveSendingResult(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        std::cout << "Send Success" << std::endl;
    }else{
        std::cout << "Send Error" << std::endl;
        QString s(reply->readAll());
        std::cout << s.toStdString();
    }
}

void Bot::sendTextMessageToUser(int chat_id,const QString&message)
{
    send_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/sendMessage?chat_id=" + QString::number(chat_id) + "&text=" + message));
    send_access_manager.get(send_request);
}

void Bot::sendDocumentToUser(int chat_id, const QString &filename)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/*"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"document\"; filename=\"" + filename + "\""));

    QFile *file = new QFile(filename);
    if(file->open(QIODevice::ReadOnly)){
        imagePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
        multiPart->append(imagePart);

        send_request.setUrl(QUrl(TELEGRAM_API_URL+"bot"+BOT_TOKEN+"/sendDocument?chat_id="+ QString::number(chat_id)));
        QNetworkReply *reply = send_access_manager.post(send_request, multiPart);
        multiPart->setParent(reply); // delete the multiPart with the reply
    }
}
