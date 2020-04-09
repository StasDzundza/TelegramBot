#include "filedownloader.h"
#include "bot.h"
#include "telegramtypesfactory.h"
#include "QFile"

TelegramFileDownloader::TelegramFileDownloader(const Bot*bot) : QObject(nullptr),bot(bot)
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    file_path_request.setSslConfiguration(sslConfiguration);
    file_data_request.setSslConfiguration(sslConfiguration);
    connect(&file_path_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveFilePath(QNetworkReply*)));
    connect(&file_data_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveFileData(QNetworkReply*)));
    connect(this, SIGNAL(sendLocalFilePath(const QString&,Update*)),this->bot, SLOT(receiveLocalFilePath(const QString&,Update*)));
}

void TelegramFileDownloader::downloadDocument(Update*update)
{
    this->update = update;
    file_path_request.setUrl(QUrl(bot->TELEGRAM_API_URL+"bot"+bot->BOT_TOKEN+"/getFile?file_id="+update->getMessage()->getDocument()->getFileId()));
    file_path_access_manager.get(file_path_request);
}

void TelegramFileDownloader::downloadPhoto(Update*update)
{
    this->update = update;
    file_path_request.setUrl(QUrl(bot->TELEGRAM_API_URL+"bot"+bot->BOT_TOKEN+"/getFile?file_id="+"TODO"));//TODO class Photo in message
    file_path_access_manager.get(file_path_request);
}

void TelegramFileDownloader::receiveFilePath(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QByteArray telegram_answer = reply->readAll();
        QJsonDocument json_document = QJsonDocument::fromJson(telegram_answer);
        QJsonObject rootObject = json_document.object();
        File *file = TelegramTypesFactory::createFile(rootObject);
        file_data_request.setUrl(bot->TELEGRAM_API_URL + "file/bot" + bot->BOT_TOKEN + '/' + file->getFilePath());
        file_data_access_manager.get(file_data_request);
    }
}

void TelegramFileDownloader::receiveFileData(QNetworkReply *reply)
{
    QString file_path = QString::number(update->getMessage()->getUser()->getId());
    QFile localFile(file_path);
    if (!localFile.open(QIODevice::WriteOnly)){
        return;
    }else{
        localFile.write(reply->readAll());
        localFile.close();
    }
    emit sendLocalFilePath(file_path,update);
}
