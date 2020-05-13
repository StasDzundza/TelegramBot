#include "filewriter.h"

#include <QFile>
#include <QTextStream>

void FileWriter::writeToFile(const QString &filepath, const QString &text)
{
    QFile qFile(filepath);
    if (qFile.open(QIODevice::WriteOnly)) {
      QTextStream out(&qFile);
      out << text;
      qFile.close();
    }
}
