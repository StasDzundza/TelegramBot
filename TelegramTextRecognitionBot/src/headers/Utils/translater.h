#ifndef TRANSLATER_H
#define TRANSLATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>

class Bot;

class Translater : public QObject
{
    Q_OBJECT
public:
    explicit Translater(Bot* bot,int user_id);

    void translateText(const QString& text,const QString&langFrom, const QString&langTo);

    void translateFile(const QString& file_path,const QString&langFrom, const QString&langTo);

signals:
    void sendTranslatedText(const QString& translated_text,int user_id);

private slots:
    void receiveTranslate(QNetworkReply*translate_reply);

private:
    Bot *bot = nullptr;
    int user_id = -1;
    QNetworkAccessManager translate_access_manager;
    QNetworkRequest translate_request;

    const QString x_rapid_host = "microsoft-azure-translation-v1.p.rapidapi.com";
    const QString x_rapid_key = "37df3f18b8msh4672e6c02bb8340p1caad0jsn60400187e4e1";
    const QString translator_url = "https://microsoft-azure-translation-v1.p.rapidapi.com/translate";
    const QString accept = "application/json";

    QString parseTranslatedTextFromXml(const QByteArray& translate_result);
};

#endif // TRANSLATER_H
