#include "socket.h"

int create_socket(const char* port) {
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