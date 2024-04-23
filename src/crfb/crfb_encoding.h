#ifndef _CRFB_ENCODING_H_
#define _CRFB_ENCODING_H_

#include "crfb_backend.h"
#include "crfb_client.h"
#include "crfb_frame_buffer.h"
#include "crfb_rectangle.h"

typedef int CRFBEncoding;

#define RAW_ENCODING 			        ((CRFBEncoding) 0)
#define COPYRECT_ENCODING 		        ((CRFBEncoding) 1)
// #define RRE_ENCODING 			        ((CRFBEncoding) 2)
// #define CORRE_ENCODING 			        ((CRFBEncoding) 4)
// #define HEXTILE_ENCODING 		        ((CRFBEncoding) 5)
#define ZLIB_ENCODING 			        ((CRFBEncoding) 6)
// #define TIGHT_ENCODING 			        ((CRFBEncoding) 7)
// #define ZLIBHEX_ENCODING 		        ((CRFBEncoding) 8)
// #define ZRLE_ENCODING 			        ((CRFBEncoding) 16)
// #define JPEG_ENCODING 			        ((CRFBEncoding) 21)
// #define OPEN_H264_ENCODING 		        ((CRFBEncoding) 50)
// #define TIGHT_PNG_ENCODING 		        ((CRFBEncoding) -260)

#define DESKTOP_SIZE_PSEUDO_ENCODING    ((CRFBEncoding) -223)

void crfb_client_recv_raw_encoding(CRFBClient* client, CRFBFramebuffer* buffer, CRFBRectangle* rect) {
    unsigned short width = rect->width;
    unsigned short height = rect->height;

    unsigned int size = width * height * 4;
    unsigned int len = 0;

    void* pixels = malloc(size);

    while (len < size) {
        int n = recv(client->socket, pixels + len, size - len, 0);

        if (n < 0) break;

        len += n;

        if(len >= size)
            break;
    }

    // Handle color corrections
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            int* pixel = &((int*) pixels)[x + y*width];

            unsigned int red =   (*pixel & 0x000000ff) >> 0;
            unsigned int green = (*pixel & 0x0000ff00) >> 8;
            unsigned int blue =  (*pixel & 0x00ff0000) >> 16;
        
            *pixel = 0xff000000 | (red << 16) | (green << 8)  | blue;
        }
    }

    for(int y = 0; y < height; ++y) {
        memcpy(
            buffer->data + ((rect->xPosition + (rect->yPosition + y) * buffer->width) * 4), 
            pixels + (y * width) * 4, 
            width * 4
        );
    }

    free(pixels);
}

void crfb_client_recv_copy_rect_encoding(CRFBClient* client, CRFBFramebuffer* buffer, CRFBRectangle* rect) {
    typedef struct CopyRectEncoding {
        unsigned short srcXPosition;
        unsigned short srcYPosition;
    } CopyRectEncoding;

    CopyRectEncoding encoding;
    int n = recv(client->socket, &encoding, sizeof(CopyRectEncoding), 0);

    if (n < 0) return;

    crfb_ushort_to_little(&encoding.srcXPosition);
    crfb_ushort_to_little(&encoding.srcYPosition);

    unsigned short width = rect->width;
    unsigned short height = rect->height;
    unsigned int size = width * height * 4;

    void* pixels = malloc(size);

    for(int y = 0; y < rect->height; ++y) {
        memcpy(
            pixels + ((y * width) * 4),
            buffer + (encoding.srcXPosition + (y + encoding.srcYPosition) * buffer->width) * 4,
            width * 4
        );
    }

    for(int y = 0; y < height; ++y) {
        memcpy(
            buffer->data + ((rect->xPosition + (rect->yPosition + y) * buffer->width) * 4), 
            pixels + (y * width) * 4, 
            width * 4
        );
        printf("COPY RECT\n");
    }

    free(pixels);
}

#endif