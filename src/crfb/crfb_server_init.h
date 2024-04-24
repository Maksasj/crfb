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

CRFBServerInit crfb_client_recv_server_init(CRFBClient* client);

void crfb_printify_server_init(CRFBServerInit serverInit);

typedef struct CRFBTightServerInitExt {
    unsigned short numberOfServerMessages;
    unsigned short numberOfClientMessages;
    unsigned short numberOfEncodings;
} CRFBTightServerInitExt;

CRFBTightServerInitExt crfb_client_recv_tight_server_init_extension(CRFBClient* client);

#endif