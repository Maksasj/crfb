#ifndef _CRFB_RECTANGLE_H_
#define _CRFB_RECTANGLE_H_

#include "crfb_client.h"
// #include "crfb_encoding.h"
#include "crfb_utils.h"

typedef struct CRFBRectangle {
    unsigned short xPosition;
    unsigned short yPosition;
    unsigned short width;
    unsigned short height;
    int ecodingType;// Todo
} CRFBRectangle;

CRFBRectangle crfb_client_recv_rectangle(CRFBClient* client) {
    CRFBRectangle rect;
    if(recv(client->socket, &rect, sizeof(CRFBRectangle), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv rectangle");

    crfb_ushort_to_little(&rect.xPosition);
    crfb_ushort_to_little(&rect.yPosition);
    crfb_ushort_to_little(&rect.width);
    crfb_ushort_to_little(&rect.height);
    crfb_int_to_little(&rect.ecodingType);

    return rect;
}

#endif