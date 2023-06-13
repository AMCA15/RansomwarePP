# RamsonwarePP

### DISCLAIMER
> This project is for testing and educational purposes only and can only be used where strict consent has been given. The author does not condone or support any illegal or unethical activities that may be carried out using this software. The author is not responsible for any damages or harm caused by the use or misuse of this software. Use at your own risk.

A simple ransomware written in C++ using the Crypto++ library.

## Getting Started

1. Clone the repository and submodule using `git clone --recurse-submodules https://github.com/AMCA15/RansomwarePP`

2. `cd RansomwarePP`

1. Generate a RSA key pair using `make generate_key`.

2. Compile the ransomware and decryptor executables using `make` command. This will also build the Crypto++ library the first time.

## Usage

### Encryption

Run the ransomware using `./build/ransomware` command. This will encrypt files in the directories `test_folder/subfolder_A`, `test_folder/subfolder_B` and its subdirectories.

A `Message.txt` file will be created in the root directory of the project with a message and a `<key_id>`. This key id is used to decrypt the files.

### Decryption

Run the decryptor creating a `key.txt` file in the root directory of the project with the `<key_id>` and running `./build/decryptor` or by running `./build/decryptor -key <key_id>` command.





