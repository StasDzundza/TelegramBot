#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "bot.h"
#include "translater.h"

Translater::Translater(Bot *bot, int user_id) : QObject(nullptr)
{
    this->bot = bot;
    this->user_id = user_id;
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    translate_request.setSslConfiguration(sslConfiguration);
    translate_request.setRawHeader(QByteArray("x-rapidapi-host"), QByteArray(x_rapid_host.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("x-rapidapi-key"), QByteArray(x_rapid_key.toStdString().c_str()));
    connect(&translate_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveTranslate(QNetworkReply*)));
    connect(this, SIGNAL(sendTranslatedText(const QString&,int)),this->bot, SLOT(receiveTranslatedText(const QString&,int)));

}

void Translater::translateText(const QString& text,const QString&langFrom, const QString &langTo)
{
    QString requestUrl = translator_url + "?source=" + langFrom + "&target=" + langTo + "&input=" + text.toUtf8();
    translate_request.setUrl(QUrl(requestUrl));
    translate_access_manager.get(translate_request);
}

void Translater::translateTextOnImage(const QImage &text, const QString &langTo)
{

}

void Translater::receiveTranslate(QNetworkReply *translate_reply)
{
    QByteArray translater_answer = translate_reply->readAll();
    QJsonDocument translate_json_doc = QJsonDocument::fromJson(translater_answer);
    QString translated_text = parseTranslatedTextFromJson(translate_json_doc);
    emit sendTranslatedText(translated_text.toUtf8(),user_id);
}

QString Translater::parseTranslatedTextFromJson(const QJsonDocument &translate_result)
{
    QJsonObject outputs = translate_result.object();
    QJsonArray output = outputs["outputs"].toArray();
    QString translated_text;
    foreach(auto&& json_value,output){
        QJsonObject translate_json_object = json_value.toObject();
        translated_text.append(translate_json_object["output"].toString());
    }
    return translated_text;
}
