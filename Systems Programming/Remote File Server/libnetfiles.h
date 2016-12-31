#ifndef _LIBNETFILES_H
#define _LIBNETFILES_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

#define unrestricted 0
#define exclusive    1
#define transaction  2
#define INVALID_FILE_MODE 9998

int initalizeSocket();
int netserverinit(char* hostNamen, int mode);
int netopen(const char* pathname, int flags);
ssize_t netread(int fildes, void* buf, size_t nbyte);
ssize_t netwrite(int fildes, const void* buf, size_t nbyte);
int netclose(int fildes);

#endif