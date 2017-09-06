#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// close
#include <unistd.h>

int main() {
  int status;
  struct addrinfo hints;
  struct addrinfo* servinfo;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  if ((status = getaddrinfo(NULL, "2000", &hints, &servinfo)) != 0) {
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
  fprintf(stdout, "%s\n", buffer);
  
  close(new_fd);
  close(sockfd);
  
  freeaddrinfo(servinfo);
  return 0;
}