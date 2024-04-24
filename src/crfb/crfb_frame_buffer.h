#ifndef _CRFB_FRAME_BUFFER_H_
#define _CRFB_FRAME_BUFFER_H_

#include <stdlib.h>

typedef struct CRFBFramebuffer {
    void* data;

    unsigned int width;
    unsigned int height;

    unsigned char isOwnData;
} CRFBFramebuffer;

CRFBFramebuffer* crfb_create_frame_buffer(unsigned int w, unsigned int h, unsigned char bytesPerPixel);

CRFBFramebuffer* crfb_create_frame_buffer_from_ptr(unsigned int w, unsigned int h, void* data);

void crfb_free_frame_buffer(CRFBFramebuffer* frameBuffer);

#endif