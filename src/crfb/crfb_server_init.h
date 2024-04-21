#ifndef _CRFB_SERVER_INIT_H_
#define _CRFB_SERVER_INIT_H_

#include "crfb_backend.h"
#include "crfb_utils.h"
#include "crfb_client.h"
#include "crfb_pixel_format.h"

typedef struct CRFBServerInit {
    unsigned short framebufferWidth;
    unsigned short framebufferHeight;

    CRFBPixelFormat pixelFormat;

    unsigned int nameLength;
    char* nameString;
} CRFBServerInit;


CRFBServerInit crfb_client_recv_server_init(CRFBClient* client) {
    CRFBServerInit serverInit;

    int len = recv(client->socket, &serverInit.framebufferWidth,    sizeof(unsigned short), 0);
    len = recv(client->socket,     &serverInit.framebufferHeight,   sizeof(unsigned short), 0);

    crfb_ushort_to_little(&serverInit.framebufferWidth);
    crfb_ushort_to_little(&serverInit.framebufferHeight);

    serverInit.pixelFormat = crfb_client_recv_pixel_format(client);

    len = recv(client->socket,     &serverInit.nameLength,   sizeof(unsigned int), 0);
    crfb_uint_to_little(&serverInit.nameLength);

    serverInit.nameString = (char*) malloc(serverInit.nameLength + 1);
    memset(serverInit.nameString, '\0', serverInit.nameLength + 1);

    len = recv(client->socket, serverInit.nameString, serverInit.nameLength, 0);

    return serverInit;
}

void crfb_printify_server_init(CRFBServerInit serverInit) {
    printf("framebufferWidth: %d\n", serverInit.framebufferWidth);
    printf("framebufferHeight: %d\n", serverInit.framebufferHeight);

    crfb_printify_pixel_format(serverInit.pixelFormat);

    printf("nameLength: %d\n", serverInit.nameLength);
    printf("nameString: %s\n", serverInit.nameString);
}

typedef struct CRFBTightServerInitExt {
    unsigned short numberOfServerMessages;
    unsigned short numberOfClientMessages;
    unsigned short numberOfEncodings;
} CRFBTightServerInitExt;

CRFBTightServerInitExt crfb_client_recv_tight_server_init_extension(CRFBClient* client) {
    CRFBTightServerInitExt ext;

    int len = recv(client->socket, &ext.numberOfServerMessages, sizeof(unsigned short), 0);
    len = recv(client->socket, &ext.numberOfClientMessages, sizeof(unsigned short), 0);
    len = recv(client->socket, &ext.numberOfEncodings, sizeof(unsigned short), 0);

    crfb_short_to_little(&ext.numberOfServerMessages);
    crfb_short_to_little(&ext.numberOfClientMessages);
    crfb_short_to_little(&ext.numberOfEncodings);

    char padding[2] = { 0 };
    len = recv(client->socket, padding, 2, 0);

    return ext;
}

#endif