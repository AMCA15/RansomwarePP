#ifndef __CRYPTO__H__
#define __CRYPTO__H__

#include "cryptopp/rsa.h"

CryptoPP::RSA::PrivateKey loadPrivateKey(void);
CryptoPP::RSA::PublicKey loadPublicKey(void);

void rsaEncrypt(CryptoPP::RSA::PublicKey& publicKey,
                std::string& plain,
                std::string& cipher);

void rsaDecrypt(CryptoPP::RSA::PrivateKey& privateKey,
                std::string& cipher,
                std::string& recovered);

void genKeyIV(CryptoPP::SecByteBlock& key, CryptoPP::SecByteBlock& iv);

void encrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             CryptoPP::byte* plain,
             CryptoPP::byte* cipher,
             size_t length);

void encrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::string& plain,
             std::string& cipher);

void encrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::istream& input,
             std::ostream& output);

void decrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             CryptoPP::byte* cipher,
             CryptoPP::byte* recovered,
             size_t length);

void decrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::string& cipher,
             std::string& recovered);

void decrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::istream& input,
             std::ostream& output);

std::string hexEncode(const CryptoPP::byte* data, size_t dataSize);
std::string hexEncode(const std::string& data);
std::string hexEncode(const CryptoPP::SecByteBlock& data);
std::string hexDecode(const std::string &input);
#endif