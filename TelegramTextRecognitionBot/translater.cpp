#include "translater.h"
#include<iostream>
#include <QDebug>
#include "bot.h"


Translater::Translater(Bot *bot, int user_id) : QObject(nullptr)
{
    this->bot = bot;
    this->user_id = user_id;
    QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    translate_request.setSslConfiguration(sslConfiguration);
    translate_request.setRawHeader(QByteArray("x-rapidapi-host"), QByteArray(x_rapid_host.toStdString().c_str()));
    translate_request.setRawHeader(QByteArray("x-rapidapi-key"), QByteArray(x_rapid_key.toStdString().c_str()));
    connect(&translate_access_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(receiveTranslate(QNetworkReply*)));
    connect(this, &Translater::sendTranslatedText,this->bot, &Bot::receiveTranslatedText);
}

void Translater::translateText(const QString& text,const QString&langFrom, const QString &langTo)
{
    QString requestText = translator_url + "?source=" + langFrom + "&target=" + langTo + "&input=" + text.toUtf8();
    std::cout << requestText.toStdString() << std::endl;
    translate_request.setUrl(QUrl(requestText));
    translate_access_manager.get(translate_request);
}

void Translater::translateTextOnImage(const QImage &text, const QString &langTo)
{

}

void Translater::receiveTranslate(QNetworkReply *translate_reply)
{
    std::cout << QString::fromUtf8(translate_reply->readAll()).toUtf8().toStdString() << std::endl;
    //emit sendTranslatedText(QString::fromUtf8(translate_reply->readAll()).toUtf8(),user_id);
}
