#pragma once
#include <network/ByteStream.h>

class PiranhaMessage {
protected:
    short version;
    ByteStream* stream;
public:
    PiranhaMessage();
    ~PiranhaMessage();
    virtual void encode();
    virtual void decode();
    virtual short getMessageType();

    ByteStream* getByteStream();
    char* getMessageBytes();
    int getEncodingLength();
    short getMessageVersion();
    void setMessageVersion(short version);
    bool isClientToServerMessage();
    bool isServerToClientMessage();
};