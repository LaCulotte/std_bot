#ifndef MESSAGETYPEHANDLER_H
#define MESSAGETYPEHANDLER_H

#include "Message.h"

class MessageTypeHandler {
public:
    MessageTypeHandler() {};
    MessageTypeHandler(const MessageTypeHandler& other) = delete;

    MessageTypeHandler& operator=(const MessageTypeHandler& other) = delete;
    virtual ~MessageTypeHandler() {};

    virtual sp<Message> generateMessageById(int id) { return nullptr; };
};

#endif