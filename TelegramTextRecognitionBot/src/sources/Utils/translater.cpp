#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "bot.h"
#include "translater.h"
#include "textreader.h"
#include <QDebug>

Translater::Translater(Bot *bot, int user_id) : QObject(nullptr),bot(bot),user_id(user_id)
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    translate_request.setSslConfiguration(sslConfiguration);
    translate_request.setRawHeader(QByteArray("x-rapidapi-host"), QByteArray(x_rapid_host.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("x-rapidapi-key"), QByteArray(x_rapid_key.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("accept-encoding"), QByteArray(accept.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("content-type"), QByteArray(content_type.toStdString().c_str()));
    connect(&translate_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveTranslate(QNetworkReply*)));
    connect(this, SIGNAL(sendTranslatedText(const QString&,int)),this->bot, SLOT(receiveTranslatedText(const QString&,int)));
}

void Translater::translateText(const QString& text,const QString&langFrom, const QString &langTo)
{
    QString requestUrl = translator_url;
    translate_request.setUrl(QUrl(requestUrl));
    QString post_data = "source="+langFrom+"&q="+text+"&target="+langTo;
    translate_access_manager.post(translate_request,post_data.toStdString().c_str());
}

void Translater::translateFile(const QString &file_path, const QString &langFrom, const QString &langTo)
{
    QString text = TextReader::readFile(file_path);
    translateText(text,langFrom,langTo);
}

void Translater::receiveTranslate(QNetworkReply *translate_reply)
{
    if(translate_reply->error() == QNetworkReply::NoError){
        QByteArray translater_answer = translate_reply->readAll();
        QJsonDocument translate_json_doc = QJsonDocument::fromJson(translater_answer);
        QString translated_text = parseTranslatedTextFromJson(translate_json_doc);
        emit sendTranslatedText(translated_text.toUtf8(),user_id);
    }else{
        emit sendTranslatedText("",user_id);
        QByteArray translater_answer = translate_reply->readAll();
        qDebug()<<translater_answer;
    }
}

QString Translater::parseTranslatedTextFromJson(const QJsonDocument &translate_result)
{
    QJsonObject outputs = translate_result.object();
    QJsonObject data = outputs["data"].toObject();
    QJsonArray translations = data["translations"].toArray();
    QString translated_text;
    for(auto&& translate_value:translations){
        QJsonObject translate_json_object = translate_value.toObject();
        translated_text+= translate_json_object["translatedText"].toString();
    }
    return translated_text;
}
