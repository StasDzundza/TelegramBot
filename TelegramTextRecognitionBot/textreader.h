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

    static QVector<QString> getFirstNWords(const QString& text,int n);

    static QString getNthWord(const QString& text,int n);

    static QString readFile(const QString& file_path);

};

#endif // TEXTREADER_H
