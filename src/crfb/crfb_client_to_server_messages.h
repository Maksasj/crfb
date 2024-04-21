#ifndef _CRFB_CLIENT_TO_SERVER_MESSAGES_H_
#define _CRFB_CLIENT_TO_SERVER_MESSAGES_H_

#include "crfb_backend.h"
#include "crfb_client.h"

#include "crfb_encoding.h"

typedef enum CRFBCLientToServerMessage {
    SET_PIXEL_FORMAT = 0,
    SET_ENCODINGS = 2,
    FRAMEBUFFER_UPDATE_REQUEST = 3,
    KEY_EVENT = 4,
    POINTER_EVENT = 5,
    CLIENT_CUT_TEXT = 6
} CRFBCLientToServerMessage;

void crfb_client_send_set_encodings_message(CRFBClient* client, CRFBEncoding* encodings, unsigned long count) {
    typedef struct SetEncodings {
        unsigned char messageType;
        unsigned char padding;
        unsigned short numberOfEncodings;
    } SetEncodings;

    SetEncodings enc;

    enc.messageType = SET_ENCODINGS;
    enc.padding = 0;
    enc.numberOfEncodings = count;
    crfb_ushort_to_little(&enc.numberOfEncodings);

    unsigned int size = sizeof(SetEncodings) + sizeof(CRFBEncoding) * count;
    unsigned char* buffer = malloc(size);

    memcpy(buffer, &enc, sizeof(SetEncodings));

    for(int i = 0; i < count; ++i) {
        CRFBEncoding encoding = encodings[i];
        crfb_uint_to_little(&encoding);        
        memcpy(buffer + sizeof(SetEncodings) + i * sizeof(CRFBEncoding), &encoding, sizeof(CRFBEncoding));
    }

    fflush(stdout);

    if(send(client->socket, buffer, size, 0) == -1)
        fprintf(stderr,"ERROR failed to send packet to server\n");

    free(buffer);
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