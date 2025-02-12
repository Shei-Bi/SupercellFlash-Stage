#include <ServerConnection.h>
#include <winsock.h>
#include <network/ClientHelloMessage.h>
#include <network/LoginMessage.h>
#include <LogicLong.hpp>
#include <GameStateManager.h>
#include <data/LogicDataTables.h>
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
#ifdef false
    messaging->connect("game.brawlstars.cn", "9339");
#else
    messaging->connect("brawl.server.dnull.xyz", "9339");

#endif
}
void ServerConnection::update(float deltaTime) {
    OwnHomeDataMessage* ohd;
    switch (state) {
    case Start:
        if (LogicVersion::isContentValidationMode()) {
            if (!LogicDataTables::isLoaded())
                break;
            state = Logined;
            ohd = new OwnHomeDataMessage();
            ohd->home = new LogicClientHome();
            ohd->home->characters.push_back(LogicDataTables::getCharacterByName("Percenter"));
            MessageManager::getInstance()->receiveMessage(ohd);
            return;
        }
        connect();
        break;
    case Connecting:
        if (messaging->connected) {
            state = Connected;
            ClientHelloMessage* c = new ClientHelloMessage();
#ifdef false
            c->clientMajor = 57;
            c->clientMinor = 0;
            c->clientBuild = 402;
            c->protocol = 2;
            c->keyVersion = 35;
            c->deviceType = 1;
            c->appStore = 1;
            c->fingerprintSha = new std::string("a29af60f08067d6f51585d4c722e455a290c7bb8");
#else
            c->clientMajor = 59;
            c->clientMinor = 1;
            c->clientBuild = 197;
            c->protocol = 2;
            c->keyVersion = 48;
            c->deviceType = 1;
            c->appStore = 1;
            c->fingerprintSha = new std::string("a302a151f0492ac0418d1775ed7a036a1eac1365");
#endif
            messaging->send(c);

            LoginMessage* l = new LoginMessage();
#ifdef false
            l->clientMajor = 57;
            l->clientMinor = 1;
            l->clientBuild = 402;
            l->fingerprintSha = new std::string("a29af60f08067d6f51585d4c722e455a290c7bb8");
            l->accountId = LogicLong::toLong(103, 6665403);
            l->token = new std::string("adx49wypcz7r3mjahk7ejgsrrhpbwknzdwb9rtw4");
#else
            l->clientMajor = 59;
            l->clientMinor = 1;
            l->clientBuild = 197;
            l->fingerprintSha = new std::string("a302a151f0492ac0418d1775ed7a036a1eac1365");
            l->accountId = LogicLong::toLong(0, 216891029);
            l->token = new std::string("HaVj/lDoyc3tPLCBczvr");
#endif
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
        MessageManager::getInstance()->update(deltaTime);
        break;
    }
}