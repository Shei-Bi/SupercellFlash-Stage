#pragma once
#include <Messaging.h>
#include <network/PiranhaMessage.h>

class MessageManager
{
public:
    MessageManager(Messaging*);
    static MessageManager* sm_pInstance;
    static MessageManager* getInstance();
    static void constructInstance(Messaging*);

    bool receiveMessage(PiranhaMessage*);

    Messaging* messaging;
};
