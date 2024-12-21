#pragma comment(lib,"ws2_32.lib")
#include <Messaging.h>
#include <thread>
#include <mutex>
// #include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <network/LogicLaserMessageFactory.hpp>
#include <stdio.h>
#include <tweetnacl.h>
#include <blake2.h>
#include <randombytes.h>
// #define SODIUM_STATIC
// #include <sodium.h>

unsigned char Messaging::spk[] = { 200,  89, 187, 144,  73,  70, 114,  69,  67, 158, 24, 147, 235, 132, 147, 208, 175, 194, 73, 228,160, 195, 165,  48, 107, 194, 153, 142, 162,  49,170,  74 };
void logChars(unsigned char* chars, int l) {
    printf("{");
    for (int i = 0;i < l;i++) {
        printf("%d, ", chars[i]);
    }
    printf("}\n");
}
int socketpair(SOCKET* shei, SOCKET* bi, int make_overlapped)
{
    union {
        struct sockaddr_in inaddr;
        struct sockaddr addr;
    } a;
    SOCKET listener;
    int e;
    socklen_t addrlen = sizeof(a.inaddr);
    DWORD flags = 0;
    int reuse = 1;

    *shei = *bi = -1;

    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
        return SOCKET_ERROR;

    memset(&a, 0, sizeof(a));
    a.inaddr.sin_family = AF_INET;
    a.inaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    a.inaddr.sin_port = 0;

    for (;;) {
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR,
            (char*)&reuse, (socklen_t)sizeof(reuse)) == -1)
            break;
        if (bind(listener, &a.addr, sizeof(a.inaddr)) == SOCKET_ERROR)
            break;

        memset(&a, 0, sizeof(a));
        if (getsockname(listener, &a.addr, &addrlen) == SOCKET_ERROR)
            break;
        // win32 getsockname may only set the port number, p=0.0005.
        // ( http://msdn.microsoft.com/library/ms738543.aspx ):
        a.inaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        a.inaddr.sin_family = AF_INET;

        if (listen(listener, 1) == SOCKET_ERROR)
            break;

        *shei = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, flags);
        if (*shei == -1)
            break;
        if (connect(*shei, &a.addr, sizeof(a.inaddr)) == SOCKET_ERROR)
            break;
        sockaddr eee;

        *bi = accept(listener, &eee, NULL);
        if (*bi == -1)
            break;

        closesocket(listener);
        return 0;
    }

    e = WSAGetLastError();
    closesocket(listener);
    closesocket(*shei);
    closesocket(*bi);
    WSASetLastError(e);
    *shei = *bi = -1;
    return SOCKET_ERROR;
}
Messaging::Messaging() {
    socketpair(&sendIn, &sendOut, 1);
    u_long mode = 1;
    ioctlsocket(sendIn, FIONBIO, &mode);
    ioctlsocket(sendOut, FIONBIO, &mode);
    connected = false;
    hasConnectFailed = false;
    connecting = false;
    disconnecting = false;
    buffer = nullptr;
    bufferSize = 0;
    cryptoState = 0;
    encrypter = nullptr;
    decrypter = nullptr;
}
void Messaging::connectToNextPort() {
    struct addrinfo* result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo("game.brawlstars.cn", "9339", &hints, &result) != 0) goto fail;

    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == -1) goto fail;

    if (::connect(sock, result->ai_addr, result->ai_addrlen) == -1) goto fail;

    if (result) freeaddrinfo(result);

    onConnect();
    // setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, true, 4);
    char kOne = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &kOne, 4);
    return;
fail:
    onConnectionFailed();
}
void Messaging::handleErrno(int err) {
    ;
}
void Messaging::wakeup() {
    char kOne = 1;
    ::send(sendIn, &kOne, 1, 0);
}
void Messaging::send(PiranhaMessage* message) {
    outgoingMessagesMutex.lock();
    outgoingMessages.push(message);
    outgoingMessagesMutex.unlock();
    wakeup();
}
void Messaging::onStart() {
    bool endure = true;
    lock.lock();
    while (endure) {
        lock.unlock();
        if (connected) {
            {
                fd_set readfds;
                FD_ZERO(&readfds);
                FD_SET(sock, &readfds);
                FD_SET(sendOut, &readfds);
                int result = select(sock + 1, &readfds, NULL, &readfds, NULL);
                if (result <= -1) handleErrno(result);
                else {
                    if (FD_ISSET(sock, &readfds)) {
                        onReceive();
                    }
                    if (FD_ISSET(sendOut, &readfds)) {
                        char b[1];
                        while (recv(sendOut, b, 1, 0) == 1);
                        onWakeup();
                    }
                }
            }
            lock.lock();
            if (!connected)
                endure = false;
            lock.unlock();
        }
        else if (connecting) {
            connectToNextPort();
        }
        lock.lock();
    }
    lock.unlock();
}
void Messaging::onConnect() {
    lock.lock();
    connected = true;
    connecting = false;
    lock.unlock();
}
void Messaging::onDisconnect() {
    lock.lock();
    connecting = false;
    lock.unlock();
}
void Messaging::onConnectionFailed() {
    hasConnectFailed = true;
}
PiranhaMessage* Messaging::nextMessage() {
    if (incomingMessages.size() == 0) return nullptr;
    incomingMessagesMutex.lock();
    PiranhaMessage* m = incomingMessages.front();
    incomingMessages.pop();
    incomingMessagesMutex.unlock();
    return m;
}
void Messaging::onWakeup() {
    while (outgoingMessages.size() > 0) {
        outgoingMessagesMutex.lock();
        PiranhaMessage* m = outgoingMessages.front();
        outgoingMessages.pop();
        outgoingMessagesMutex.unlock();
        if (m->getEncodingLength() == 0) m->encode();
        int EncodingLength = m->getEncodingLength() + 7;
        if (EncodingLength > bufferSize) {
            if (buffer) delete(buffer);
            buffer = new char[EncodingLength];
            bufferSize = EncodingLength;
        }
        memcpy(buffer + 7, m->getByteStream()->getByteArray(), m->getEncodingLength());
        writeHeader(m, buffer, m->getEncodingLength());
        writeBlocking(buffer, EncodingLength);
        delete(m);
    }
}
void Messaging::writeBlocking(void* buf, int length) {
    ::send(sock, (char*)buf, length, 0);
}
void Messaging::writeHeader(PiranhaMessage* m, void* buf, int length) {
    printf("send message of type %d length %d\n", m->getMessageType(), m->getEncodingLength());
    short messageType = m->getMessageType();
    short messageVersion = m->getMessageVersion();

    char* buffer = (char*)buf;
    buffer[0] = (char)(messageType >> 8);
    buffer[1] = (char)(messageType);
    buffer[2] = (char)(length >> 16);
    buffer[3] = (char)(length >> 8);
    buffer[4] = (char)(length);
    buffer[5] = (char)(messageVersion >> 8);
    buffer[6] = (char)(messageVersion);
}
void Messaging::onReceive() {
    char* header = new char[7];
    if (!readBlocking(header, 7)) {
        return;
    }
    // printf("header: {%d, %d, %d, %d, %d, %d, %d}\n", header[0], header[1], header[2], header[3], header[4], header[5], header[6]);
    unsigned short type = ((header[0] & 0xFF) << 8) | (header[1] & 0xFF);
    unsigned int length = ((header[2] & 0xFF) << 16) | ((header[3] & 0xFF) << 8) | (header[4] & 0xFF);
    unsigned short version = ((header[5] & 0xFF) << 8) | (header[6] & 0xFF);
    delete[] header;
    char* rest = new char[length];
    PiranhaMessage* m = LogicLaserMessageFactory::createMessageByType(type);
    if (!readBlocking(rest, length)) {
        return;
    }
    if (decrypter != nullptr) {
        if (decrypter->decrypt(rest, rest, length)) abort();
        length -= decrypter->getEncryptionOverhead();
    }
    if (!m) {
        delete[] rest;
        printf("Ignoring message of unknown type %d\n", type);
        return;
    }
    printf("receive message of type %d\n", type);
    m->setMessageVersion(version);
    m->getByteStream()->setByteArray(rest, length);
    if (cryptoState == 2) {
        handlePepperLoginResponse(m);
    }
    m->decode();
    if (type == 20100) {
        sendPepperLogin((ServerHelloMessage*)m);
        return;
    }
    incomingMessagesMutex.lock();
    incomingMessages.push(m);
    incomingMessagesMutex.unlock();
}
void box(unsigned char* in, int insize, unsigned char* out, unsigned char* nonce, unsigned char* spk, unsigned char* csk) {
    unsigned char* gCryptoScratch = new unsigned char[insize + 32];
    memset(gCryptoScratch, 0, 32);
    memcpy(gCryptoScratch + 32, in, insize);
    crypto_box_curve25519xsalsa20poly1305_tweet(gCryptoScratch, gCryptoScratch, insize + 32, nonce, spk, csk);
    memcpy(out, gCryptoScratch + 16, insize + 16);
}
bool box_open(unsigned char* in, int insize, unsigned char* out, unsigned char* nonce, unsigned char* spk, unsigned char* csk) {
    unsigned char* gCryptoScratch = new unsigned char[insize + 16];
    memset(gCryptoScratch, 0, 16);
    memcpy(gCryptoScratch + 16, in, insize);
    int result = crypto_box_curve25519xsalsa20poly1305_tweet_open(gCryptoScratch, gCryptoScratch, insize + 16, nonce, spk, csk);
    if (!result) {
        memcpy(out, gCryptoScratch + 32, insize - 16);
    }
    return result;
}
void Messaging::sendPepperLogin(ServerHelloMessage* m) {
    cryptoState = 2;
    randombytes(encryptNonce, 24);
    encryptNonce[0] &= 0xFE;
    crypto_box_keypair(cpk, csk);
    unsigned char nonce[24];
    blake2b_state hash;
    blake2b_init(&hash, 24);
    blake2b_update(&hash, cpk, 32);
    blake2b_update(&hash, spk, 32);
    blake2b_final(&hash, nonce, 24);

    pendingLoginMessage->encode();
    int len = 24 + 24 + pendingLoginMessage->getEncodingLength();
    unsigned char* unciphered = new unsigned char[len];
    unsigned char* ciphered = new unsigned char[32 + len + 16];
    unsigned char* ServerHelloToken = (unsigned char*)m->removeServerHelloToken();
    memcpy(unciphered, ServerHelloToken, 24);
    memcpy(unciphered + 24, encryptNonce, 24);
    memcpy(unciphered + 48, pendingLoginMessage->getMessageBytes(), pendingLoginMessage->getEncodingLength());

    box(unciphered, len, ciphered + 32, nonce, spk, csk);

    memcpy(ciphered, cpk, 32);
    pendingLoginMessage->getByteStream()->setByteArray((char*)ciphered, 32 + len + 16);
    pendingLoginMessage->getByteStream()->setOffset(32 + len + 16);
    send(pendingLoginMessage);

    delete[] unciphered;
    delete[] ServerHelloToken;
    // delete pendingLoginMessage; //deleted by onWakeup
    pendingLoginMessage = nullptr;
}
void Messaging::handlePepperLoginResponse(PiranhaMessage* m) {
    unsigned char nonce[24];
    blake2b_state hash;
    blake2b_init(&hash, 24);
    blake2b_update(&hash, encryptNonce, 24);
    blake2b_update(&hash, cpk, 32);
    blake2b_update(&hash, spk, 32);
    blake2b_final(&hash, nonce, 24);

    int len = m->getByteStream()->getLength() - 16;
    unsigned char* unciphered = new unsigned char[len];

    if (box_open((unsigned char*)m->getByteStream()->getByteArray(), len + 16, unciphered, nonce, spk, csk)) {
        abort();
    }

    memcpy(decryptNonce, unciphered, 24);
    unsigned char sharedKey[32];
    memcpy(sharedKey, unciphered + 24, 32);

    unsigned char* payload = new unsigned char[len - 56];
    memcpy(payload, unciphered + 56, len - 56);
    m->getByteStream()->setByteArray((char*)payload, len - 56);

    decrypter = new PepperEncrypter(sharedKey, decryptNonce);
    encrypter = new PepperEncrypter(sharedKey, encryptNonce);

    cryptoState = 3;

    delete[] unciphered;
}
bool Messaging::readBlocking(void* buf, int length) {
    if (length == 0) return true;
    int result = recv(sock, (char*)buf, length, MSG_WAITALL);
    if (result <= -1) {
        handleErrno(result);
        return false;
    }
    else if (result == 0) close();
    return result == length;
}
void Messaging::close() {
    if (connected) {
        closesocket(sock);
        onDisconnect();
    }
    sock = -1;
    connected = false;
}
void threadFunc(Messaging* m) {
    m->onStart();
}
void Messaging::connect(char*, char*) {
    connecting = true;
    // connected = true;
    hasConnectFailed = false;
    std::thread thread = std::thread(&threadFunc, this);
    thread.detach();
}