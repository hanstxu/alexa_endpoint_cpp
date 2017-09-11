#ifndef SOCKET_H
#define SOCKET_H

// general utility functions
#include <stdio.h>
#include <stdlib.h>

// memset
#include <string.h>

// socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int create_socket(const char* port);

#endif