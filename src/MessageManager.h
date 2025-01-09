#pragma once
#include <Messaging.h>
#include <network/PiranhaMessage.h>
class UdpLaserSocket;

class MessageManager
{
public:
    MessageManager(Messaging*);
    static MessageManager* sm_pInstance;
    static MessageManager* getInstance();
    static void constructInstance(Messaging*);

    bool receiveMessage(PiranhaMessage*);

    bool sendMessage(PiranhaMessage*);

    void update(float deltaTime);

    Messaging* messaging;
    UdpLaserSocket* udpSocket;
    bool isUdpConnectionInfoMessageReceived;
};
