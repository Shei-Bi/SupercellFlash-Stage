#pragma once
#include "PiranhaMessage.h"
#include "ByteStream.h"
#include "LogicLaserMessageFactory.hpp"

class UdpSingleMessagePacket {
public:
    PiranhaMessage* message;

    UdpSingleMessagePacket() {
        message = nullptr;
    }

    ~UdpSingleMessagePacket() {
        ;
    }

    void encode(ByteStream* stream, bool encodeMsg) {
        stream->writeVInt(message->getMessageType());
        if (encodeMsg) message->encode();
        stream->writeVInt(message->getEncodingLength());
        stream->writeBytesWithoutLength((unsigned char*)message->getByteStream()->getByteArray(), message->getEncodingLength());// not writeBytes()!!!!!! i spend 5 hours fixing this
    }

    bool decode(unsigned char* buffer, int length) {
        if (length > 1990) return false;
        ByteStream bytestream(buffer, length);
        short type = bytestream.readVInt();
        message = LogicLaserMessageFactory::createMessageByType(type);
        if (!message) {
            printf("UdpMessage::decode unable to read message type %d\n", type);
            return false;
        }
        int messageLength = bytestream.readVInt();
        if (messageLength != length - bytestream.getOffset()) {
            printf("UdpMessage::decode didn't read whole message properly. %d -> %d\n", messageLength, length - bytestream.getOffset());
            return false;
        }
        message->getByteStream()->setByteArray(bytestream.readBytes(messageLength, 900000), messageLength);
        if (message->getMessageType() == 24109) {
            return ((VisionUpdateMessage*)message)->decodeUDP(messageLength);
        }
        else {
            message->decode();
            return true;
        }
    }
};