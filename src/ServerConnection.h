#pragma once
#include <Messaging.h>

class ServerConnection {
public:
    enum NetworkState {
        Start = 0,
        Connecting = 1,
        Connected = 3,
        Logining = 4,
        Logined = 8
    };
    ServerConnection();
    static ServerConnection* sm_pInstance;
    static ServerConnection* getInstance();
    static void constructInstance();
    int state;
    Messaging* messaging;
    void update(float deltaTime);
    void connect();
    void connectTo(char*, char*);
};