#pragma once
#include "stdio.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "MessageManager.h"
#include "network/UdpSingleMessagePacket.h"

class UdpLaserSocket {
public:
    SOCKET sock;
    sockaddr_in addr;
    unsigned char buffer[1400];
    unsigned char sessionId[10];
    UdpLaserSocket() {
        sock = -1;
        ZeroMemory(&addr, sizeof(addr));
    }
    bool connect(const char* addr, int port, unsigned char* sessionId, unsigned char* nonce) {
        unsigned long kOne = 1;
        sockaddr_in local;
        int l = sizeof(local);
        ZeroMemory(&local, l);

        printf("UdpSocket::connect %s : %d\nSessionId { ", addr, port);
        for (int i = 0;i < 10;i++) printf("%d, ", sessionId[i]);
        printf(" }\nNonce { ");
        for (int i = 0;i < 0;i++) printf("%d, ", nonce[i]);
        printf(" }\n");

        if (sock != -1) closesocket(sock);
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == SOCKET_ERROR) goto fail;
        this->addr.sin_family = AF_INET;
        this->addr.sin_port = htons(port);
        this->addr.sin_addr.S_un.S_addr = inet_addr(addr);

        local.sin_family = AF_INET;

        if ((bind(sock, (sockaddr*)&local, l)) != 0) goto fail;

        if (inet_addr(addr) == INADDR_NONE) goto fail;

        if (ioctlsocket(sock, FIONBIO, &kOne) != 0) goto fail;

        memcpy(this->sessionId, sessionId, 10);

        return true;
    fail:
        if (sock != SOCKET_ERROR) closesocket(sock);
        assert(false);
        return false;
    }
    void update(MessageManager* listener, float sinceStart, float deltaTime) {
        if (sock == -1) return;
        sockaddr_in remote;
        int l = sizeof(remote);
        ZeroMemory(&remote, l);

        for (int i = 0;i < 10;i++) {
            int received = recvfrom(sock, (char*)buffer, 1400, 0, (sockaddr*)&remote, &l);
            if (received == SOCKET_ERROR) {
                int error = WSAGetLastError();
                if (error != WSAEWOULDBLOCK) printf("error: %d\n", error);
            }
            if (received <= 0) return;
            else if (received < 10) {
                printf("Received too small udp packet\n");
                continue;
            }
            else if (received > 1400) {
                printf("Received too big udp packet\n");
                continue;
            }
            if (remote.sin_port != addr.sin_port) {
                close();
                return;
            }
            for (int i = 0;i < 10;i++) {
                if (sessionId[i] != buffer[i]) {
                    printf("UdpPacket session id mismatch\n");
                    continue;
                }
            }
            UdpSingleMessagePacket udpSingleMessagePacket = UdpSingleMessagePacket();
            if (udpSingleMessagePacket.decode(buffer + 10, received - 10))
                listener->receiveMessage(udpSingleMessagePacket.message);
            else if (udpSingleMessagePacket.message) delete udpSingleMessagePacket.message;
        }
    }

    bool send(char* buffer, int length) {
        int required = length + 10;
        assert(required <= 1400);
        memcpy(this->buffer, sessionId, 10);
        memcpy(this->buffer + 10, buffer, length);
        //45,136,228,68,192,48,50,231,123,3,187,164,1,10,0,0,0,1,0,0,0,2,0,0
        int result = sendto(sock, (char*)this->buffer, required, 0, (sockaddr*)&addr, sizeof(addr));
        if (result == SOCKET_ERROR) printf("sendto(): error: %d\n", WSAGetLastError());
        return result == required;
    }

    void close() {
        if (sock != -1) ::closesocket(sock);
        sock = -1;
    }
};