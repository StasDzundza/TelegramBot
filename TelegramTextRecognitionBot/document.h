#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QString>

class Document
{
public:
    Document();

    QString getFileId()const;
    void setFileId(QString file_id);

    int getFileSize()const;
    void setFileSize(int file_size);

    QString getFileName()const;
    void setFileName(QString file_name);

    QString getMimeType()const;
    void setMimeType(QString mime_type);
private:
    int file_size;
    QString file_name,mime_type,file_id;
};

#endif // DOCUMENT_H
