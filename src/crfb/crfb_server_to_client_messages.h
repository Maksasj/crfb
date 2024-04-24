#ifndef _CRFB_SERVER_TO_CLIENT_MESSAGES_H_
#define _CRFB_SERVER_TO_CLIENT_MESSAGES_H_

#include "crfb_backend.h"
#include "crfb_client.h"
#include "crfb_encoding.h"
#include "crfb_utils.h"

typedef enum CRFBServerToClientMessage {
    FRAMEBUFFER_UPDATE = 0,         // [x]
    SET_COLOUR_MAP_ENTRIES = 1,     // [ ]
    BELL = 2,                       // [ ]
    SERVER_CUT_TEXT = 3             // [ ]
} CRFBServerToClientMessage;

typedef struct CRFBFramebufferUpdate {
    unsigned char messageType;
    unsigned char padding;
    unsigned short numberOfRectangles;
} CRFBFramebufferUpdate;

void crfb_client_recv_framebuffer_update_message(CRFBClient* client, CRFBFramebufferUpdate*);

#endif