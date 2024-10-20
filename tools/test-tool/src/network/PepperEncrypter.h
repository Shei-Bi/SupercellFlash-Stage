#pragma once

class PepperEncrypter {
public:
    int encrypt(char* input, char* output, int length);
    int decrypt(char* input, char* output, int length);
    int getEncryptionOverhead();
};