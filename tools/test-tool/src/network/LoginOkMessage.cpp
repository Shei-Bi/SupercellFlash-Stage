#include <network/LoginOkMessage.h>
#include <string.h>

LoginOkMessage::LoginOkMessage() {
    token = new std::string();
}

LoginOkMessage::~LoginOkMessage() {
    ;
}

void LoginOkMessage::decode() {
    accountId = stream->readLongLong();
    homeId = stream->readLongLong();
    token = stream->readString();
}

short LoginOkMessage::getMessageType() {
    return 20104;
}