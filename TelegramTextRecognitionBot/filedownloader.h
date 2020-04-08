#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Bot;

class TelegramFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit TelegramFileDownloader(Bot*bot,int user_id);

    void downloadFile(const QString&file_id);

signals:
    void sendLocalFilePath(const QString&,int);

private slots:
    void receiveFileData(QNetworkReply*);

    void receiveFilePath(QNetworkReply*);

private:
    Bot *bot;
    int user_id;

    QNetworkAccessManager file_path_access_manager;
    QNetworkRequest file_path_request;

    QNetworkAccessManager file_data_access_manager;
    QNetworkRequest file_data_request;

    void getFilePath(const QString&file_id);
};

#endif // FILEDOWNLOADER_H
