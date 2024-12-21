#include <network/PiranhaMessage.h>

PiranhaMessage::PiranhaMessage() {
    this->stream = new ByteStream(10);
    this->version = 0;
}

PiranhaMessage::~PiranhaMessage() {
    delete this->stream;
}

ByteStream* PiranhaMessage::getByteStream() {
    return this->stream;
}

char* PiranhaMessage::getMessageBytes() {
    return this->stream->getByteArray();
}

int PiranhaMessage::getEncodingLength() {
    return this->stream->getOffset();
}

short PiranhaMessage::getMessageVersion() {
    return this->version;
}

void PiranhaMessage::setMessageVersion(short version) {
    this->version = version;
}

bool PiranhaMessage::isClientToServerMessage() {
    short type = this->getMessageType();
    return (type >= 10000 && type < 20000) || type == 30000;
}

bool PiranhaMessage::isServerToClientMessage() {
    short type = this->getMessageType();
    return (type >= 20000 && type < 30000) || type == 40000;
}

void PiranhaMessage::encode() {
    ;
}

void PiranhaMessage::decode() {
    ;
}

short PiranhaMessage::getMessageType() {
    return 0;
}