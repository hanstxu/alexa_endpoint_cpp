#include "ssl.h"

void init_openssl() {
  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() {
  EVP_cleanup();
}

SSL_CTX* create_context() {
  const SSL_METHOD* method;
  SSL_CTX* ctx;
  
  method = SSLv23_server_method();
  
  ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Error: Unable to create SSL context\n");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
  
  return ctx;
}

void configure_context(SSL_CTX* ctx, const char* cert_file,
  const char* key_file) {
  // apparently, this is deprecated?
  //SSL_CTX_set_ecdh_auto(ctx, 1);
  
  // Set the key and certificate
  if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM)
    <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
  
  if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM)
    <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}