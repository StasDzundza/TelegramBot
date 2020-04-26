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

    void translateTextOnImage(const QImage&text, const QString&langTo);

signals:
    void sendTranslatedText(const QString& translated_text,int user_id);

private slots:
    void receiveTranslate(QNetworkReply*translate_reply);

private:
    Bot *bot = nullptr;
    int user_id = -1;
    QNetworkAccessManager translate_access_manager;
    QNetworkRequest translate_request;

    const QString x_rapid_host = "systran-systran-platform-for-language-processing-v1.p.rapidapi.com";
    const QString x_rapid_key = "38ad4062ebmsh87a6e9de422268cp1a2e0ejsn6e9d57cfa81c";
    const QString translator_url = "https://systran-systran-platform-for-language-processing-v1.p.rapidapi.com/translation/text/translate";

    QString parseTranslatedTextFromJson(const QJsonDocument& translate_result);
};

#endif // TRANSLATER_H
