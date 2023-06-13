#ifndef __CONFIG__H__
#define __CONFIG__H__

// This file is intended to override the default configuration of the
// ransomware/decrypto program. Any changes made to this file will take
// precedence over the default configuration.

#define FILES_EXTENSIONS                                                      \
  {                                                                           \
    ".mp3", ".ogg", ".wav", ".wma", ".7z", ".rar", ".tar.gz", ".zip", ".txt", \
        ".html", ".css", ".js", ".py", ".pdf", ".png", ".xlsx", ".docx",      \
        ".pptx", ".jpeg"                                                      \
  }

#include "default_config.h"
#endif