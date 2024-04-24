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

CRFBPixelFormat crfb_client_recv_pixel_format(CRFBClient* client);

void crfb_printify_pixel_format(CRFBPixelFormat format);

#endif