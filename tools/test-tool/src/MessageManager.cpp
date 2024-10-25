#include <MessageManager.h>
#include <network/LoginOkMessage.h>
#include <stdio.h>
#include <LogicLong.hpp>
#include <ServerConnection.h>
#include <GameStateManager.h>
#include <network/OwnHomeDataMessage.hpp>

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
    }
}