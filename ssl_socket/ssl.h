#ifndef SSL_H
#define SSL_H

// openssl
#include <openssl/ssl.h>
#include <openssl/err.h>

void init_openssl();

void cleanup_openssl();

SSL_CTX* create_context();

void configure_context(SSL_CTX* ctx, const char* cert_file,
  const char* key_file);

#endif