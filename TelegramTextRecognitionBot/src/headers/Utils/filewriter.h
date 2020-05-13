#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>

class FileWriter
{
public:
    FileWriter()=default;

    static void writeToFile(const QString& filepath,const QString& text);
};

#endif // FILEWRITER_H
