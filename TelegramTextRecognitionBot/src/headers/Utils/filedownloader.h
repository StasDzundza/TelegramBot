#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "update.h"

class Bot;

class TelegramFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit TelegramFileDownloader(const Bot*bot);

    void downloadDocument(Update*update);

    void downloadPhoto(Update*update);

signals:
    void sendLocalFilePath(const QString&,Update*);

private slots:
    void receiveFileData(QNetworkReply*);

    void receiveFilePath(QNetworkReply*);

private:
    const Bot *bot;
    Update*update;

    QNetworkAccessManager file_path_access_manager;
    QNetworkRequest file_path_request;

    QNetworkAccessManager file_data_access_manager;
    QNetworkRequest file_data_request;

    void getFilePath(const QString&file_id);
};

#endif // FILEDOWNLOADER_H
