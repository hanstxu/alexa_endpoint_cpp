// general utility functions
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

// ssl
#include <openssl/ssl.h>
#include <openssl/err.h>

// close
#include <unistd.h>

// Http classes
#include "HttpRequest.h"
#include "HttpResponse.h"

#include "alexa.h"

using namespace std;

#define BUFFER_SIZE 2048

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

void configure_context(SSL_CTX* ctx) {
  // apparently, this is deprecated?
  //SSL_CTX_set_ecdh_auto(ctx, 1);
  
  // Set the key and certificate
  if (SSL_CTX_use_certificate_file(ctx, "certificate.pem", SSL_FILETYPE_PEM)
    <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
  
  if (SSL_CTX_use_PrivateKey_file(ctx, "private-key.pem", SSL_FILETYPE_PEM)
    <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

int create_socket(char* port) {
  int sockfd;
  
  struct addrinfo hints;
  struct addrinfo* server_info;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET; // supports only IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = AI_PASSIVE; // Fill in ip address automatically
  
  if (getaddrinfo(NULL, port, &hints, &server_info) < 0) {
    perror("Error: Unable to getaddrinfo\n");
    exit(EXIT_FAILURE);
  }
  
  // ai_protocol = 0 (auto)
  sockfd = socket(server_info->ai_family, server_info->ai_socktype,
    server_info->ai_protocol);
  if (sockfd < 0) {
    perror("Error: Unable to create socket\n");
    exit(EXIT_FAILURE);
  }
  
  if (bind(sockfd, server_info->ai_addr, server_info->ai_addrlen) < 0) {
    perror("Error: Unable to bind to port\n");
    exit(EXIT_FAILURE);
  }
  
  if (listen(sockfd, 1) < 0) {
    perror("Error: Unable to listen to port\n");
    exit(EXIT_FAILURE);
  }
  
  return sockfd;
}

std::string createHttpResponse(JSONObject response) {
  HttpResponse http_res = HttpResponse("HTTP/1.1", "200", "OK");
  http_res.addHeader("Server", "CustomServer/1.0");
  http_res.addHeader("Content-Type", "application/json");
  http_res.addHeader("Content-Length", to_string(response.toString().size()));
  http_res.addHeader("Connection", "Closed");
  http_res.addBody(response.toString());
  return http_res.toString();
}

void setup_server(char* port) {
  int sockfd;
  SSL_CTX* ctx;
  
  init_openssl();
  ctx = create_context();
  
  configure_context(ctx);
  
  sockfd = create_socket(port);
  
  struct sockaddr_in addr;
  unsigned int addr_size = sizeof addr;
  SSL* ssl;
  
  int client_fd = accept(sockfd, (struct sockaddr *)&addr, &addr_size);
  if (client_fd < 0) {
    perror("Error: Unable to accept client connection\n");
    exit(EXIT_FAILURE);
  }
  
  ssl = SSL_new(ctx);
  SSL_set_fd(ssl, client_fd);
  
  if (SSL_accept(ssl) < 0)
    ERR_print_errors_fp(stderr);
  
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  SSL_read(ssl, buffer, BUFFER_SIZE);
  buffer[BUFFER_SIZE - 1] = 0;
  
  std::string str(buffer);
  HttpRequest http_req = HttpRequest(str);
  JSONObject request = parseJSON<JSONObject>(http_req.getBody());
  JSONObject response = invokeSkill(request);
  std::string http_res = createHttpResponse(response);
  cout << http_res << endl;
  
  SSL_write(ssl, http_res.c_str(), http_res.size());

  close(client_fd);
  close(sockfd);
  cleanup_openssl();
}

// temp
/*
#include <fstream>

std::string fileString(std::string filename) {
  std::ifstream t(filename);
std::string str;

t.seekg(0, std::ios::end);   
str.reserve(t.tellg());
t.seekg(0, std::ios::beg);

str.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());	
  return str;
}
*/

int main(int argc, char** argv) {
  setup_server(argv[1]);
  /*std::string test = fileString("input1.txt");
  JSONObject request = parseJSON<JSONObject>(HttpRequest(test).getBody());;
  JSONObject response = invokeSkill(request);
  HttpResponse http_res = HttpResponse("HTTP/1.1", "200", "OK");
  http_res.addHeader("Server", "CustomServer/1.0");
  http_res.addHeader("Content-Type", "application/json");
  http_res.addHeader("Content-Length", to_string(response.toString().size()));
  http_res.addHeader("Connection", "Closed");
  http_res.addBody(response.toString());
  cout << http_res.toString() << endl;*/
  return 0;
}