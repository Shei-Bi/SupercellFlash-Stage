#include <MessageManager.h>
#include <network/LoginOkMessage.h>
#include <stdio.h>
#include <LogicLong.hpp>
#include <ServerConnection.h>
#include <GameStateManager.h>
#include <network/OwnHomeDataMessage.hpp>
#include "network/UdpConnectionInfoMessage.h"
#include "network/StartLoadingMessage.h"
#include "UdpLaserSocket.h"
#include "BattleMode.h"
#include "battle/LogicBattleModeClient.h"
#include "network/ClientInfoMessage.h"
#include "battle/ClientInputManager.h"

MessageManager* MessageManager::sm_pInstance = nullptr;
MessageManager* MessageManager::getInstance() {
    return sm_pInstance;
}
MessageManager::MessageManager(Messaging* m) {
    messaging = m;
    isUdpConnectionInfoMessageReceived = false;
    udpSocket = nullptr;
}
void MessageManager::constructInstance(Messaging* m)
{
    MessageManager::sm_pInstance = new MessageManager(m);
}
bool MessageManager::receiveMessage(PiranhaMessage* m) {
    BattleMode* BattleMode = nullptr;
    switch (m->getMessageType()) {
    case 20104:
        printf("Logined as %d-%d, token: %s\n", LogicLong::getHigherInt(((LoginOkMessage*)m)->accountId), LogicLong::getLowerInt(((LoginOkMessage*)m)->accountId), ((LoginOkMessage*)m)->token->c_str());

        ServerConnection::getInstance()->state = ServerConnection::Logined;
        return true;
    case 24101:
        GameStateManager::getInstance()->setGameData(((OwnHomeDataMessage*)m)->home, ((OwnHomeDataMessage*)m)->avatar);
        return true;
    case 20559:
        if (udpSocket) delete udpSocket;
        udpSocket = nullptr;
        isUdpConnectionInfoMessageReceived = false;
        GameStateManager::getInstance()->changeState(GameStateManager::Battle);
        GameStateManager::getInstance()->changeToState();

        sendMessage(new ClientInfoMessage());
        BattleMode = BattleMode::getInstance();
        BattleMode->battleClient->ownPlayerIndex = ((StartLoadingMessage*)m)->ownPlayerIndex;
        BattleMode->battleClient->ownTeamIndex = ((StartLoadingMessage*)m)->ownTeamIndex;
        BattleMode->battleClient->setGameModeVariationData(((StartLoadingMessage*)m)->gameModeVariation);
        BattleMode->battleClient->setLocation(((StartLoadingMessage*)m)->location);
        BattleMode->battleClient->setPlayerCount(((StartLoadingMessage*)m)->players.size());
        for (auto i : ((StartLoadingMessage*)m)->players) BattleMode->battleClient->addPlayer(i->clone());
        BattleMode->battleClient->generateTileMap();
        BattleMode->battleClient->startLoadingReceived = true;
        return true;
    case 24112:
        udpSocket = new UdpLaserSocket();
        isUdpConnectionInfoMessageReceived = true;
        if (!udpSocket->connect(((UdpConnectionInfoMessage*)m)->addr, ((UdpConnectionInfoMessage*)m)->port, ((UdpConnectionInfoMessage*)m)->sessionId, ((UdpConnectionInfoMessage*)m)->nonce)) {
            printf("Failed to open UDP socket\n");
            delete udpSocket;
            udpSocket = nullptr;
        }
        delete[]((UdpConnectionInfoMessage*)m)->addr;
        if (((UdpConnectionInfoMessage*)m)->sessionId) delete[]((UdpConnectionInfoMessage*)m)->sessionId;
        if (((UdpConnectionInfoMessage*)m)->nonce) delete[]((UdpConnectionInfoMessage*)m)->nonce;
        return true;
    case 24109:
        // srand(time(0));
        // if (rand() % 10 == 0) return true;
        BattleMode = BattleMode::getInstance();
        if (BattleMode == nullptr) return true;
        if (BattleMode->battleClient->addVisionUpdate((VisionUpdateMessage*)m)) {
            BattleMode->inputManager->handleVisionUpdate((VisionUpdateMessage*)m);
            return false;
        }
        else return true;
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

void MessageManager::update(float deltaTime) {
    if (udpSocket) udpSocket->update(this, 0.0f, deltaTime);
}
