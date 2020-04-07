#include <sstream>
#include "textreader.h"

QVector<QString> TextReader::splitTextByWords(const QString &text)
{
    QVector<QString> words;
    std::string command = text.toStdString();
    // Used to split string around spaces.
    std::istringstream ss(command);
    // Traverse through all words
    do {
        // Read a word
        std::string word;
        ss >> word;
        if(!word.empty()){
            words << QString::fromStdString(word);
        }
    } while (ss);
    return words;
}

QString TextReader::getTextAfterNthWord(const QString &text,int word_num)
{
    QString text_after_nth_word;
    int num_of_chars_before_word = 0;
    std::string command = text.toStdString();
    std::istringstream ss(command);
    int tmp_num = word_num;
    while(tmp_num-- && ss){
        std::string word;
        ss >> word;
        if(!word.empty()){
           num_of_chars_before_word+=word.size();
        }
    }
    num_of_chars_before_word+=word_num; //spaces
    if(text.size() > num_of_chars_before_word){
        std::copy(text.begin() + num_of_chars_before_word,text.end(),std::back_inserter(text_after_nth_word));
    }
    return text_after_nth_word;
}
