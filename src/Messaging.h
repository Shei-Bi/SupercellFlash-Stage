#pragma once
#include <Winsock2.h>
#include <mutex>
#include <network/PiranhaMessage.h>
#include <queue>
#include <network/PepperEncrypter.h>
#include <network/ServerHelloMessage.h>

class Messaging {
public:
    Messaging();

    void connect(const char*, const char*);
    void onStart();
    void onConnect();
    void onDisconnect();
    void onWakeup();
    void onReceive();
    void onConnectionFailed();
    void connectToNextPort();
    void handleErrno(int);
    void send(PiranhaMessage*);
    void wakeup();
    static void writeHeader(PiranhaMessage*, void*, int);
    void writeBlocking(void*, int);
    bool readBlocking(void*, int);
    void close();
    void sendPepperLogin(ServerHelloMessage*);
    void handlePepperLoginResponse(PiranhaMessage*);
    PiranhaMessage* nextMessage();

    const char* addr;
    const char* port;

    bool connected;
    bool hasConnectFailed;

    std::mutex lock;
    SOCKET sock;
    SOCKET sendIn;
    SOCKET sendOut;
    bool connecting;
    bool disconnecting;

    std::queue<PiranhaMessage*> outgoingMessages;
    std::mutex outgoingMessagesMutex;
    std::queue<PiranhaMessage*> incomingMessages;
    std::mutex incomingMessagesMutex;

    char* buffer;
    int bufferSize;

    int cryptoState;
    PepperEncrypter* encrypter;
    PepperEncrypter* decrypter;

    unsigned char encryptNonce[24];
    unsigned char decryptNonce[24];
    unsigned char cpk[32];
    unsigned char csk[32];
    static unsigned char spk[32];

    PiranhaMessage* pendingLoginMessage;
};