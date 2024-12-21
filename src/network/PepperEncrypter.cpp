#include <network/PepperEncrypter.h>
#include <tweetnacl.h>
#include <memory.h>

void box(unsigned char* in, int insize, unsigned char* out, unsigned char* nonce, unsigned char* key) {
    unsigned char* gCryptoScratch = new unsigned char[insize + 32];
    memset(gCryptoScratch, 0, 32);
    memcpy(gCryptoScratch + 32, in, insize);
    crypto_box_curve25519xsalsa20poly1305_tweet_afternm(gCryptoScratch, gCryptoScratch, insize + 32, nonce, key);
    memcpy(out, gCryptoScratch + 16, insize + 16);
}
bool box_open(unsigned char* in, int insize, unsigned char* out, unsigned char* nonce, unsigned char* key) {
    unsigned char* gCryptoScratch = new unsigned char[insize + 16];
    memset(gCryptoScratch, 0, 16);
    memcpy(gCryptoScratch + 16, in, insize);
    bool result = crypto_box_curve25519xsalsa20poly1305_tweet_open_afternm(gCryptoScratch, gCryptoScratch, insize + 16, nonce, key);
    if (!result) {
        memcpy(out, gCryptoScratch + 32, insize - 16);
    }
    return result;
}
PepperEncrypter::PepperEncrypter(unsigned char* k, unsigned char* n) {
    memcpy(key, k, 32);
    memcpy(nonce, n, 24);
}
int PepperEncrypter::getEncryptionOverhead() {
    return 16;
}
bool PepperEncrypter::decrypt(char* in, char* out, int length) {
    unsigned char add = 2;
    for (int i = 0;i < 24;i++) {
        int sum = nonce[i] + add;
        nonce[i] = (unsigned char)sum;
        add = (unsigned char)(sum / 256);
    }
    return box_open((unsigned char*)in, length, (unsigned char*)out, nonce, key);
}