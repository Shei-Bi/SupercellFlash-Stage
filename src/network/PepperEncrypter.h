#pragma once

class PepperEncrypter {
public:
    PepperEncrypter(unsigned char* k, unsigned char* n);
    bool encrypt(char* input, char* output, int length);
    bool decrypt(char* input, char* output, int length);
    int getEncryptionOverhead();
    unsigned char key[32];
    unsigned char nonce[24];
};