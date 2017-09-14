// close
#include <unistd.h>

// Http classes
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"

// Essential functions
#include "ssl_socket/ssl.h"
#include "ssl_socket/socket.h"
#include "alexa.h"

// to check if the port parameter is only numbers
#include <ctype.h>

// to check if cert_file and key_file exists
#include <fstream>

#define BUFFER_SIZE 2048

void check_args(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Error: expected 4 arguments\n");
    exit(EXIT_FAILURE);
  }
  
  int i = 0;
  while (argv[1][i] != '\0') {
    if (!isdigit(argv[1][i++])) {
      fprintf(stderr, "Error: port argument contains characters that are not");
	  fprintf(stderr, " digits\n");
      exit(EXIT_FAILURE);
    }
  }
  
  std::ifstream infile(argv[2]);
  if (!infile.is_open()) {
    fprintf(stderr, "Error: certificate file does not exist\n");
    exit(EXIT_FAILURE);
  }
  infile.close();
  
  infile.open(argv[3]);
  if (!infile.is_open()) {
    fprintf(stderr, "Error: private key file does not exist\n");
    exit(EXIT_FAILURE);
  }
  infile.close();
}

std::string createHttpSuccessResponse(JSONObject response) {
  HttpResponse http_res = HttpResponse("HTTP/1.1", "200", "OK");
  http_res.addHeader("Server", "CustomServer/1.0");
  http_res.addHeader("Content-Type", "application/json");
  http_res.addHeader("Content-Length", std::to_string(response.toString().size()));
  http_res.addHeader("Connection", "Closed");
  http_res.addBody(response.toString());
  return http_res.toString();
}

std::string createHttpUnauthorizedResponse(JSONObject response) {
  HttpResponse http_req = HttpResponse("HTTP/1.1", "401", "Unauthorized");
  return http_res.to_string();
}

void setup_server(const char* port, const char* cert_file,
  const char* key_file) {
  int sockfd;
  SSL_CTX* ctx;
  
  init_openssl();
  ctx = create_context();
  
  configure_context(ctx, cert_file, key_file);
  
  sockfd = create_socket(port);
  
  struct sockaddr_in addr;
  unsigned int addr_size = sizeof addr;
  SSL* ssl;
  
  while (1) {
    int client_fd = accept(sockfd, (struct sockaddr *)&addr, &addr_size);
    if (client_fd < 0) {
      fprintf(stderr, "Error: Unable to accept client connection\n");
      exit(EXIT_FAILURE);
    }
    
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);
    
    if (SSL_accept(ssl) < 0) {
      ERR_print_errors_fp(stderr);
      exit(EXIT_FAILURE);
    }
    
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    SSL_read(ssl, buffer, BUFFER_SIZE);
    buffer[BUFFER_SIZE - 1] = 0;
    
    std::string str(buffer);
    HttpRequest http_req = HttpRequest(str);
    JSONObject request = parseJSON<JSONObject>(http_req.getBody());
    JSONObject response = invokeSkill(request);
    std::string http_res;
	
	// makes sure that other Alexa developers are not allowed to use this web
    // service
    if (response.toString() == "{}")
      http_res = createHttpUnauthorizedResponse(response);
    else
      http_res = createHttpSuccessResponse(response);
  
    SSL_write(ssl, http_res.c_str(), http_res.size());

    close(client_fd);
  }
  
  close(sockfd);
  cleanup_openssl();
}

int main(int argc, char** argv) {
  check_args(argc, argv);
  setup_server(argv[1], argv[2], argv[3]);
  exit(EXIT_SUCCESS);
}