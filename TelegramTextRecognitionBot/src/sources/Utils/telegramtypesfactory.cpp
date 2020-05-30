#include <QJsonArray>
#include <QJsonDocument>

#include "telegramtypesfactory.h"

QVector<Update *> TelegramTypesFactory::parseUpdates(const QJsonObject &updates)
{
    QVector<Update *> updates_vector;
    QJsonArray result = updates["result"].toArray();
    foreach(auto&& json_value,result){
        QJsonObject update_json_object = json_value.toObject();
        Update *update = createUpdate(update_json_object);
        updates_vector.push_back(update);
    }
    return updates_vector;
}

Update *TelegramTypesFactory::createUpdate(const QJsonObject &update_json_object)
{
    Update* update = new Update();
    int update_id = update_json_object["update_id"].toInt();
    Message *message = createMessage(update_json_object["message"].toObject());
    update->setUpdateId(update_id);
    update->setMessage(*message);
    return update;
}

Message *TelegramTypesFactory::createMessage(const QJsonObject &message_json_object){
    Message* message = new Message();
    int message_id = message_json_object["message_id"].toInt();
    QString text = message_json_object["text"].toString();
    QString caption = message_json_object["caption"].toString();
    User *user = createUser(message_json_object["from"].toObject());
    Document *doc = createDocument(message_json_object["document"].toObject());
    message->setMessageId(message_id);
    message->setUser(*user);
    message->setDocument(*doc);
    message->setText(text);
    message->setCaption(caption);
    QJsonArray photos = message_json_object["photo"].toArray();
    int best_quality_photo_index = photos.size() - 1;
    QJsonObject photo_json_object = photos.takeAt(best_quality_photo_index - 1).toObject();
    message->setPhotoId(photo_json_object["file_id"].toString());
    return message;
}

User *TelegramTypesFactory::createUser(const QJsonObject &user_json_object){
    User *user = new User();
    int id = user_json_object["id"].toInt();
    QString username = user_json_object["username"].toString();
    user->setId(id);
    user->setUsername(username);
    return user;
}

File *TelegramTypesFactory::createFile(const QJsonObject &file_json_object)
{
    File *file = new File();
    QJsonObject file_object = file_json_object["result"].toObject();
    QString file_id = file_object["file_id"].toString();
    QString file_path = file_object["file_path"].toString();
    int file_size = file_object["file_size"].toInt();
    file->setFileId(file_id);
    file->setFilePath(file_path);
    file->setFileSize(file_size);
    return file;
}

Document *TelegramTypesFactory::createDocument(const QJsonObject &document_json_object)
{
    Document*doc = new Document();
    QString file_name = document_json_object["file_name"].toString();
    QString mime_type = document_json_object["mime_type"].toString();
    QString file_id = document_json_object["file_id"].toString();
    int file_size = document_json_object["file_size"].toInt();
    doc->setFileName(file_name);
    doc->setMimeType(mime_type);
    doc->setFileId(file_id);
    doc->setFileSize(file_size);
    return doc;
}

QString TelegramTypesFactory::buildJsonCommandKeyboardObject(const QVector<QString> &buttons)
{
    QJsonObject keyboard_json;
    QJsonArray keyboard_buttons;
    for(auto&command:buttons){
        QJsonObject button;
        button.insert("text",command);
        QJsonArray button_array;
        button_array.push_back(button);
        keyboard_buttons.push_back(button_array);
    }
    keyboard_json.insert("keyboard",keyboard_buttons);
    QJsonDocument doc(keyboard_json);
    return doc.toJson();
}
