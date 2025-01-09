#pragma once
#include "network/ClientInputMessage.h"
#include "MessageManager.h"
#include "network/UdpSingleMessagePacket.h"
#include "UdpLaserSocket.h"
#include "ClientInput.h"

class ClientInputManager {
private:
    static constexpr float sendInputInterval = 0.05f;
    static constexpr float keepAliveInterval = 1.0f;
    float updateTimer;
    int inputIndex;
    std::vector<ClientInput*> outgoingInputs;
public:
    ClientInputManager() {
        updateTimer = 0.0f;
        inputIndex = 0;
    }
    void update(float sinceStart, float deltaTime) {
        updateTimer -= deltaTime;
        if (updateTimer <= 0.0f) {
            auto clientInputMessage = new ClientInputMessage();
            clientInputMessage->tickCounter = 0;
            clientInputMessage->index = ++inputIndex;
            if (inputIndex >= 8000) inputIndex = 0;
            auto udpSocket = MessageManager::getInstance()->udpSocket;
            if (udpSocket) {
                auto udpSingleMessagePacket = UdpSingleMessagePacket();
                udpSingleMessagePacket.message = clientInputMessage;
                auto byteStream = ByteStream(128);
                udpSingleMessagePacket.encode(&byteStream, true);
                udpSocket->send(byteStream.getByteArray(), byteStream.getOffset());
                delete clientInputMessage;
                updateTimer += (outgoingInputs.size() == 0 ? keepAliveInterval : sendInputInterval);
            }
            else abort();
        }
    }
};