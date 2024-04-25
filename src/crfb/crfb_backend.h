#ifndef _CRFB_BACKEND_H_
#define _CRFB_BACKEND_H_


#ifdef _WIN32
    #include <winsock2.h>
    #define socklen_t int
#else
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netinet/tcp.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "crfb_logger.h"

#endif