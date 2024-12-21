#include <ServerConnection.h>
#include <winsock.h>
#include <network/ClientHelloMessage.h>
#include <network/LoginMessage.h>
#include <LogicLong.hpp>
#include <GameStateManager.h>
#include <LogicDataTables.h>
#include <MessageManager.h>
#include <LogicVersion.h>
#include <network/OwnHomeDataMessage.hpp>

ServerConnection* ServerConnection::sm_pInstance = nullptr;
ServerConnection* ServerConnection::getInstance() {
    return sm_pInstance;
}
ServerConnection::ServerConnection() {
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    messaging = new Messaging();
    state = Start;

    MessageManager::constructInstance(messaging);
}
void ServerConnection::constructInstance()
{
    if (!ServerConnection::sm_pInstance)
        ServerConnection::sm_pInstance = new ServerConnection();
}
void ServerConnection::connect() {
    state = Connecting;
    messaging->connect("game.brawlstars.cn", "9339");
}
void ServerConnection::update(float deltaTime) {
    switch (state) {
    case Start:
        if (LogicVersion::isContentValidationMode()) {
            state = Logined;
            MessageManager::getInstance()->receiveMessage(new OwnHomeDataMessage());
            return;
        }
        connect();
        break;
    case Connecting:
        if (messaging->connected) {
            state = Connected;
            ClientHelloMessage* c = new ClientHelloMessage();
            c->clientMajor = 57;
            c->clientMinor = 0;
            c->clientBuild = 402;
            c->protocol = 2;
            c->keyVersion = 35;
            c->deviceType = 1;
            c->appStore = 1;
            c->fingerprintSha = new std::string("a29af60f08067d6f51585d4c722e455a290c7bb8");
            messaging->send(c);

            LoginMessage* l = new LoginMessage();
            l->clientMajor = 57;
            l->clientMinor = 1;
            l->clientBuild = 402;
            l->fingerprintSha = new std::string("a29af60f08067d6f51585d4c722e455a290c7bb8");
            l->accountId = LogicLong::toLong(103, 6665403);
            l->token = new std::string("adx49wypcz7r3mjahk7ejgsrrhpbwknzdwb9rtw4");
            messaging->pendingLoginMessage = l;
        }
        break;
    case Connected:
    case Logining:
    case Logined:
        if (!GameStateManager::getInstance()->isChangingMode() && LogicDataTables::isLoaded()) {
            PiranhaMessage* m;
            while ((m = messaging->nextMessage(), m != nullptr)) {
                if (MessageManager::getInstance()->receiveMessage(m)) delete m;
            }
        }
        break;
    }
}