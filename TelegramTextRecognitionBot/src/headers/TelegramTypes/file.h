#ifndef FILE_H
#define FILE_H

#include <QString>

class File
{
public:
    File();
    QString getFileId() const;
    void setFileId(const QString &value);

    int getFileSize() const;
    void setFileSize(int value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

private:
    QString file_id;
    int file_size = 0;
    QString file_path;
};

#endif // FILE_H
