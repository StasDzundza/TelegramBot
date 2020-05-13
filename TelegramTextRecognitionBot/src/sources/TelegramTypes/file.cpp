#include "file.h"

QString File::getFileId() const
{
    return file_id;
}

void File::setFileId(const QString &value)
{
    file_id = value;
}

int File::getFileSize() const
{
    return file_size;
}

void File::setFileSize(int value)
{
    file_size = value;
}

QString File::getFilePath() const
{
    return file_path;
}

void File::setFilePath(const QString &value)
{
    file_path = value;
}
