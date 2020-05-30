#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "bot.h"
#include "translater.h"
#include "textreader.h"
#include <QDebug>
#include <QXmlStreamReader>

Translater::Translater(Bot *bot, int user_id) : QObject(nullptr),bot(bot),user_id(user_id)
{
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    translate_request.setSslConfiguration(sslConfiguration);
    translate_request.setRawHeader(QByteArray("x-rapidapi-host"), QByteArray(x_rapid_host.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("x-rapidapi-key"), QByteArray(x_rapid_key.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("accept"), QByteArray(accept.toStdString().c_str()));
    connect(&translate_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveTranslate(QNetworkReply*)));
    connect(this, SIGNAL(sendTranslatedText(const QString&,int)),this->bot, SLOT(receiveTranslatedText(const QString&,int)));
}

void Translater::translateText(const QString& text,const QString&langFrom, const QString &langTo)
{
    QString requestUrl = translator_url + "?from=" + langFrom + "&to="+langTo+"&text=" + text;
    translate_request.setUrl(QUrl(requestUrl));
    translate_access_manager.get(translate_request);
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
        QString translated_text = parseTranslatedTextFromXml(translater_answer);
        qDebug()<<translated_text;
        emit sendTranslatedText(translated_text.toUtf8(),user_id);
    }else{
        emit sendTranslatedText("",user_id);
        QByteArray translater_answer = translate_reply->readAll();
        qDebug()<<translater_answer;
    }
}

QString Translater::parseTranslatedTextFromXml(const QByteArray &translate_result)
{
    QXmlStreamReader reader(translate_result);
    while(!reader.atEnd() && !reader.hasError()) {
        if(reader.readNext() == QXmlStreamReader::StartElement && reader.name() == "string") {
            return  reader.readElementText();
        }
    }
}
