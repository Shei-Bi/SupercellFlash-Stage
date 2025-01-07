#include <MessageManager.h>
#include <network/LoginOkMessage.h>
#include <stdio.h>
#include <LogicLong.hpp>
#include <ServerConnection.h>
#include <GameStateManager.h>
#include <network/OwnHomeDataMessage.hpp>
#include "network/UdpConnectionInfoMessage.h"

MessageManager* MessageManager::sm_pInstance = nullptr;
MessageManager* MessageManager::getInstance() {
    return sm_pInstance;
}
MessageManager::MessageManager(Messaging* m) {
    messaging = m;
}
void MessageManager::constructInstance(Messaging* m)
{
    MessageManager::sm_pInstance = new MessageManager(m);
}
bool MessageManager::receiveMessage(PiranhaMessage* m) {
    switch (m->getMessageType()) {
    case 20104:
        printf("Logined as %d-%d, token: %s\n", LogicLong::getHigherInt(((LoginOkMessage*)m)->accountId), LogicLong::getLowerInt(((LoginOkMessage*)m)->accountId), ((LoginOkMessage*)m)->token->c_str());

        ServerConnection::getInstance()->state = ServerConnection::Logined;
        return true;
    case 24101:
        GameStateManager::getInstance()->setGameData(((OwnHomeDataMessage*)m)->home, ((OwnHomeDataMessage*)m)->avatar);
        return true;
    case 24112:
        auto msg = ((UdpConnectionInfoMessage*)m);
        printf("UdpSocket::connect %s : %d\nSessionId { ", msg->addr, msg->port);
        for (int i = 0;i < msg->sessionIdLength;i++) printf("%d, ", msg->sessionId[i]);
        printf(" }\nNonce { ");
        for (int i = 0;i < msg->nonceLength;i++) printf("%d, ", msg->nonce[i]);
        printf(" }\n");
        return true;
    }
}

bool MessageManager::sendMessage(PiranhaMessage* m) {
    if (messaging->cryptoState != 3) {
        printf("sending message type %d when unauthenticated!\n", m->getMessageType());
        delete m;
        return false;
    }
    messaging->send(m);
    return true;
}
