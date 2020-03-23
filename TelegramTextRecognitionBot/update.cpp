#include "update.h"

Update::Update()
{

}

int Update::getUpdateId() const
{
    return update_id;
}

void Update::setUpdateId(int value)
{
    update_id = value;
}

void Update::setMessage(const Message &message)
{
    this->message = std::make_shared<Message>(message);
}

Message *Update::getMessage() const
{
    return message.get();
}

QString Update::toString()
{
    QString update_info;
    update_info += "Update id = " + QString::number(update_id) + '\n';
    update_info += "Message info : " + message->toString() + '\n';
    return update_info;
}

