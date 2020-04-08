#include "filedownloader.h"
#include "bot.h"
#include "telegramtypesfactory.h"
#include "QFile"

TelegramFileDownloader::TelegramFileDownloader(Bot*bot,int user_id) : QObject(nullptr),bot(bot),user_id(user_id)
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    file_path_request.setSslConfiguration(sslConfiguration);
    file_data_request.setSslConfiguration(sslConfiguration);
    connect(&file_path_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveFilePath(QNetworkReply*)));
    connect(&file_data_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveFileData(QNetworkReply*)));
    connect(this, SIGNAL(sendFilePath(const QString&,int)),this->bot, SLOT(receiveLocalFilePath(const QString&,int)));
}

void TelegramFileDownloader::downloadFile(const QString &file_id)
{
    file_path_request.setUrl(QUrl(bot->TELEGRAM_API_URL+"bot"+bot->BOT_TOKEN+"/getFile?file_id="+file_id));
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
    QString file_path = QString::number(user_id);
    QFile localFile(file_path);
    if (!localFile.open(QIODevice::WriteOnly)){
        return;
    }else{
        localFile.write(reply->readAll());
        localFile.close();
    }
    emit sendLocalFilePath(file_path,user_id);
}
