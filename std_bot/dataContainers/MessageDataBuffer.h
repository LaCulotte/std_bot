#ifndef MESSAGEDATABUFFER_H
#define MESSAGEDATABUFFER_H

#include "DataBuffer.h"
#include "Logger.h"

class MessageDataBuffer : public DataBuffer<char> {
public:

    MessageDataBuffer() {};
    MessageDataBuffer(vector<char> dat) : DataBuffer<char>(dat) {};
    MessageDataBuffer(char *dat, int size) : DataBuffer<char>(dat, size) {};

    MessageDataBuffer(const MessageDataBuffer& other) : DataBuffer<char>(other) {};
    MessageDataBuffer& operator=(const MessageDataBuffer& other) = default;
    ~MessageDataBuffer() = default;


    char * toCharArray();

    bool readBool();
    char readByte();
    int16_t readShort();
    int16_t readVarShort();
    int readInt();
    int readVarInt();
    string readUTF();
    string readUTFBytes(int length);

    void writeBool(bool b);
    void writeByte(char byte);
    void writeShort(int16_t s);
    void writeVarShort(int16_t s);
    void writeInt(int i);
    void writeVarInt(int i);
    void writeUTF(string UTF);
    void writeUTFBytes(string UTF);

    void print();
};

#endif