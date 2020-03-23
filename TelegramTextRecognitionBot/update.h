#ifndef UPDATE_H
#define UPDATE_H

#include "message.h"

class Update
{
public:
    Update();

    int getUpdateId() const;
    void setUpdateId(int value);

    void setMessage(const Message&message);
    Message* getMessage()const;

private:
    int update_id = -1;
    std::shared_ptr<Message> message;
};

#endif // UPDATE_H
