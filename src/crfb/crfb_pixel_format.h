#ifndef _CRFB_PIXEL_FORMAT_H_
#define _CRFB_PIXEL_FORMAT_H_

#include "crfb_backend.h"
#include "crfb_client.h"

typedef struct CRFBPixelFormat {
    unsigned char bitsPerPixel;
    unsigned char depth;
    unsigned char bigEndianFlag;
    unsigned char trueColorFlag;

    unsigned short redMax;
    unsigned short greenMax;
    unsigned short blueMax;

    unsigned char redShift;
    unsigned char greenShift;
    unsigned char blueShift;
} CRFBPixelFormat;

CRFBPixelFormat crfb_client_recv_pixel_format(CRFBClient* client) {
    CRFBPixelFormat format;

    if(recv(client->socket, &format.bitsPerPixel,    sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format depth");
    
    if(recv(client->socket,     &format.depth,           sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format depth");
    
    if(recv(client->socket,     &format.bigEndianFlag,   sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format bigEndianFlag");

    if(recv(client->socket,     &format.trueColorFlag,   sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format trueColorFlag");

    if(recv(client->socket,     &format.redMax,          sizeof(unsigned short), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format redMax");

    if(recv(client->socket,     &format.greenMax,        sizeof(unsigned short), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format greenMax");

    if(recv(client->socket,     &format.blueMax,         sizeof(unsigned short), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format blueMax");

    if(recv(client->socket,     &format.redShift,        sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format redShift");

    if(recv(client->socket,     &format.greenShift,      sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format greenShift");

    if(recv(client->socket,     &format.blueShift,       sizeof(unsigned char), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format blueShift");

    crfb_ushort_to_little(&format.redMax);
    crfb_ushort_to_little(&format.greenMax);
    crfb_ushort_to_little(&format.blueMax);

    char padding[3] = { 0 };
    if(recv(client->socket, padding, 3, 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv pixel format padding");

    return format;
}

void crfb_printify_pixel_format(CRFBPixelFormat format) {
    printf("bitsPerPixel: %d\n", format.bitsPerPixel);
    printf("depth: %d\n", format.depth);
    printf("bigEndianFlag: %d\n", format.bigEndianFlag);
    printf("trueColorFlag: %d\n", format.trueColorFlag);
    printf("redMax: %d\n", format.redMax);
    printf("greenMax: %d\n", format.greenMax);
    printf("blueMax: %d\n", format.blueMax);
    printf("redShift: %d\n", format.redShift);
    printf("greenShift: %d\n", format.greenShift);
    printf("blueShift: %d\n", format.blueShift);
}

#endif