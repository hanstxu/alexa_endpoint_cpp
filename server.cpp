#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// close
#include <unistd.h>

// Http classes
#include "HttpRequest.h"

void setup_server() {
    int status;
  struct addrinfo hints;
  struct addrinfo* servinfo;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  if ((status = getaddrinfo(NULL, "443", &hints, &servinfo)) != 0) {
    fprintf(stderr, "Error: ");
    exit(EXIT_FAILURE);
  }
  
  int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
    servinfo->ai_protocol);
  
  status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
  
  // will change the backlog to something else
  listen(sockfd, 1);

  struct sockaddr_storage their_addr;
  socklen_t addr_size = sizeof their_addr;
  
  int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
  
  char buffer[1024];
  memset(buffer, 0, 1024);
  int num_bytes = recv(new_fd, buffer, 1024, 0);
  std::string req(buffer);
  HttpRequest test = HttpRequest(req);
  fprintf(stdout, "%s\n", test.toString().c_str());
  
  close(new_fd);
  close(sockfd);
  
  freeaddrinfo(servinfo);
}

int main() {
  setup_server();
  //HttpRequest test = HttpRequest("GET / HTTP/1.1\r\nstuff:key\r\nhello:world\r\n\r\nhello");
  //fprintf(stdout, "%s\n", test.toString().c_str());
  return 0;
}