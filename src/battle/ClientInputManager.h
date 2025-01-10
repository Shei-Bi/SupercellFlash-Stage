#pragma once
#include "network/ClientInputMessage.h"
#include "MessageManager.h"
#include "network/UdpSingleMessagePacket.h"
#include "UdpLaserSocket.h"
#include "ClientInput.h"
#include "network/VisionUpdateMessage.h"

class ClientInputManager {
private:
    static constexpr float resendInputInterval = 0.05f;
    static constexpr float keepAliveInterval = 1.0f;
    float updateTimer;
    int inputPacketIndex;
    int inputIndex;
    std::vector<ClientInput*> outgoingInputs;
public:
    ClientInputManager() {
        updateTimer = 0.0f;
        inputPacketIndex = 0;
        inputIndex = 0;
    }
    void addInput(ClientInput* input) {
        outgoingInputs.push_back(input);
        input->index = ++inputIndex;
        updateTimer = 0.0f;
    }
    void update(float sinceStart, float deltaTime) {
        updateTimer -= deltaTime;
        if (updateTimer <= 0.0f) {
            auto clientInputMessage = new ClientInputMessage();
            clientInputMessage->tickCounter = 0;
            clientInputMessage->index = ++inputPacketIndex;
            if (inputPacketIndex >= 8000) inputPacketIndex = 0;
            auto udpSocket = MessageManager::getInstance()->udpSocket;
            if (udpSocket) {
                auto udpSingleMessagePacket = UdpSingleMessagePacket();
                udpSingleMessagePacket.message = clientInputMessage;
                auto byteStream = ByteStream(128);
                udpSingleMessagePacket.encode(&byteStream, true);
                udpSocket->send(byteStream.getByteArray(), byteStream.getOffset());
                delete clientInputMessage;
                updateTimer += (outgoingInputs.size() == 0 ? keepAliveInterval : resendInputInterval);
            }
            else abort();
        }
    }

    void handleVisionUpdate(VisionUpdateMessage* message) {
        for (int i = 0;i < outgoingInputs.size();i++) {
            if (outgoingInputs[i]->index <= message->handledInputs) {
                delete outgoingInputs[i];
                outgoingInputs.erase(outgoingInputs.begin() + i);
            }
        }
        if (outgoingInputs.size() == 0) updateTimer = keepAliveInterval;

    }
};