#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMap>
#include <QSet>
#include "update.h"

class Bot : public QObject
{
    Q_OBJECT
public:
    Bot();
    void start();
    friend class TelegramFileDownloader;

public slots:
    void receiveTranslatedText(const QString&translated_text,int user_id);

    void receiveLocalFilePath(const QString&local_file_path,Update*update);

private slots:
    void receiveUpdates(QNetworkReply*reply);

    void receiveSendingResult(QNetworkReply*reply);

private:
    const QString BOT_TOKEN = "1055976603:AAHeHiEW4QLNKqGduZazziVToKlRe_gSN-8";
    const QString TELEGRAM_API_URL = "https://api.telegram.org/";

    const QString INVALID_COMMAND = "INVALID COMMAND. Write /commands and check valid and existing commands.";
    const QString INVALID_TYPE_OF_TEXT_FILE = "Invalide type of file. It should be text file, not image.";
    const QString INVALID_TYPE_OF_PHOTO_FILE = "Invalide type of file. It should be image.";

    const QSet<QString> VALID_COMMANDS{"/start","/translate_text","/translate_file","/recognize_photo","/write_text_to_file"};

    QMap<int,QString> last_user_commands;

    QNetworkAccessManager update_access_manager;
    QNetworkRequest update_request;

    QNetworkAccessManager send_access_manager;
    QNetworkRequest send_request;

    void processUpdate(Update*update);

    void executeUserCommand(Update*update);

    void sendTextMessageToUser(int chat_id,const QString& message);

    void sendDocumentToUser(int chat_id,const QString& filename);

    void sendReplyToUserCommand(const Update*update);

    void sendCommandKeyboardToUser(int chat_id);
};

#endif // BOT_H
