#pragma once

#include<pbc++/PBC.h>
#include<iostream>
#include<vector>
#include <random>
#include"cryptopp/sha.h"
#include<cryptopp/rsa.h>
#include<cryptopp/aes.h>
#include<cryptopp/cryptlib.h>
#include<cryptopp/osrng.h>
#include<cryptopp/modes.h>
#include<cryptopp/hex.h>
#include<cryptopp/filters.h>
#include<functional>

CryptoPP::byte *GenRandomAesCipher();

class OperatorTester{
private:
    int testTimes;
    //ECC opt arg
    Pairing e;
    std::vector<G1> G1Args;
    std::vector<G2> G2Args;
    std::vector<string> strArgs;
    std::vector<Zr> ZrArgs;
    vector<CryptoPP::byte *> byteBufferArgs;
    //AES key
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
    vector<CryptoPP::byte *> AesCiphers;
    // RSA key
    CryptoPP::RSA::PrivateKey sk;
    CryptoPP::RSA::PublicKey pk;
    vector<CryptoPP::byte *> RsaCiphers;
    string getRandomString(int len);
    //Opt alg s
    void PointMul(int i);
    void PointAdd(int i);
    void Hash(int i);
    void SymEnc(int i);
    void SymDec(int i);
    void RsaEnc(int i);
    void RsaDec(int i);
    void EccPairing(int i);
    auto TestAlg(std::function<void(int)> f);

public:
    OperatorTester(int times, FILE* paramFile);
    ~OperatorTester();
    void run();
};
