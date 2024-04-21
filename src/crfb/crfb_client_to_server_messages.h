#ifndef _CRFB_CLIENT_TO_SERVER_MESSAGES_H_
#define _CRFB_CLIENT_TO_SERVER_MESSAGES_H_

#include "crfb_backend.h"
#include "crfb_client.h"

typedef enum CRFBCLientToServerMessage {
    SET_PIXEL_FORMAT = 0,
    SET_ENCODINGS = 2,
    FRAMEBUFFER_UPDATE_REQUEST = 3,
    KEY_EVENT = 4,
    POINTER_EVENT = 5,
    CLIENT_CUT_TEXT = 6
} CRFBCLientToServerMessage;

void crfb_client_send_set_encodings_message(CRFBClient* client) {
    // Todo
    // Magic packet, probably set encodings
    /*
    char message[40] = {
        0x02, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 
        0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 
        0xff, 0xff, 0xff, 0x21, 0xff, 0xff, 0xff, 0x20, 
        0xff, 0xff, 0xff, 0x18, 0xff, 0xff, 0xff, 0x11,
    }; 

    if(send(client->socket, &message, 40, 0) == -1) {
        fprintf(stderr,"ERROR failed to send packet to server\n");
    }
    */

    char message[36] = {
        0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 
        0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 
        0xff, 0xff, 0xff, 0x21, 0xff, 0xff, 0xff, 0x20, 
        0xff, 0xff, 0xff, 0x18//, 0xff, 0xff, 0xff, 0x11
    }; 

    if(send(client->socket, &message, 36, 0) == -1) {
        fprintf(stderr,"ERROR failed to send packet to server\n");
    }
}

void crfb_client_send_framebuffer_update_request_message(
    CRFBClient* client, 
    unsigned char incremental, 
    unsigned short xPosition,
    unsigned short yPosition,
    unsigned short width,
    unsigned short height
) {
    typedef struct FramebufferUpdateRequest {
        unsigned char messageType;
        unsigned char incremental;

        unsigned short xPosition;
        unsigned short yPosition;
        unsigned short width;
        unsigned short height;
    } FramebufferUpdateRequest;

    FramebufferUpdateRequest req;
    req.messageType = FRAMEBUFFER_UPDATE_REQUEST;
    req.incremental = incremental;

    req.xPosition = xPosition;
    req.yPosition = yPosition;
    req.width = width;
    req.height = height;

    crfb_ushort_to_little(&req.xPosition);
    crfb_ushort_to_little(&req.yPosition);
    crfb_ushort_to_little(&req.width);
    crfb_ushort_to_little(&req.height);

    if(send(client->socket, &req, sizeof(FramebufferUpdateRequest), 0) == -1)
        fprintf(stderr,"ERROR failed to send packet to server\n");
}

#endif