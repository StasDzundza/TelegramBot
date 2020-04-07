#ifndef TEXTREADER_H
#define TEXTREADER_H

#include <QVector>
#include <QString>

class TextReader
{
public:
    TextReader() = default;

    static QVector<QString> splitTextByWords(const QString& text);

    static QString getTextAfterNthWord(const QString& text,int word_num);
};

#endif // TEXTREADER_H
