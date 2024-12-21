#include "LoginMessage.h"

LoginMessage::LoginMessage() {
    this->fingerprintSha = new std::string();
    this->token = new std::string;
    clientMajor = 0;
    clientMinor = 0;
    clientBuild = 0;
}

LoginMessage::~LoginMessage() {
    delete this->fingerprintSha;
    delete this->token;
}

void LoginMessage::encode() {
    stream->writeLongLong(accountId);
    stream->writeString(token);

    stream->writeInt(clientMajor);
    stream->writeInt(clientMinor);
    stream->writeInt(clientBuild);
    stream->writeString(fingerprintSha);

    stream->writeString(new std::string("iPhone6,2"));
    stream->writeDataReference();
    stream->writeString(NULL);
    stream->writeString(NULL);
    stream->writeBoolean(false);
    stream->writeString(NULL);
    stream->writeString(NULL);
    stream->writeBoolean(false);
    stream->writeString(NULL);
    stream->writeInt(0);
    stream->writeVInt(0);
    stream->writeString(new std::string("57.402"));

    stream->writeString(new std::string("05293EE6CD815561D6F99CBB12E49B26"));
    stream->writeString(new std::string("DFD8E28312222BB0CAACBC5E9852450C"));
    stream->writeVInt(2);

    //idk
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
    stream->writeInt(0);
}

short LoginMessage::getMessageType() {
    return 10101;
}