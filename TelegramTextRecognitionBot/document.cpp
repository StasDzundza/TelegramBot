#include "document.h"

Document::Document()
{

}

QString Document::getFileId() const
{
    return file_id;
}

void Document::setFileId(QString file_id)
{
    this->file_id = file_id;
}

int Document::getFileSize() const
{
    return file_size;
}

void Document::setFileSize(int file_size)
{
    this->file_size = file_size;
}

QString Document::getFileName() const
{
    return file_name;
}

void Document::setFileName(QString file_name)
{
    this->file_name = file_name;
}

QString Document::getMimeType() const
{
    return mime_type;
}

void Document::setMimeType(QString mime_type)
{
    this->mime_type = mime_type;
}
