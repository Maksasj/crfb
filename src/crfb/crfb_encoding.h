#ifndef _CRFB_ENCODING_H_
#define _CRFB_ENCODING_H_

#include "crfb_backend.h"
#include "crfb_client.h"
#include "crfb_frame_buffer.h"

typedef int CRFBEncoding;

#define RAW_ENCODING 			        ((CRFBEncoding) 0)
#define COPYRECT_ENCODING 		        ((CRFBEncoding) 1)
#define RRE_ENCODING 			        ((CRFBEncoding) 2)
#define CORRE_ENCODING 			        ((CRFBEncoding) 4)
#define HEXTILE_ENCODING 		        ((CRFBEncoding) 5)
#define ZLIB_ENCODING 			        ((CRFBEncoding) 6)
#define TIGHT_ENCODING 			        ((CRFBEncoding) 7)
#define ZLIBHEX_ENCODING 		        ((CRFBEncoding) 8)
#define ZRLE_ENCODING 			        ((CRFBEncoding) 16)
#define JPEG_ENCODING 			        ((CRFBEncoding) 21)
#define OPEN_H264_ENCODING 		        ((CRFBEncoding) 50)
#define TIGHT_PNG_ENCODING 		        ((CRFBEncoding) -260)

#define DESKTOP_SIZE_PSEUDO_ENCODING    ((CRFBEncoding) -223)

void crfb_client_recv_raw_encoding(CRFBClient* client, CRFBFramebuffer* buffer) {
    unsigned int size = buffer->width * buffer->height * 4;
    unsigned int len = 0;

    while (len < size) {
        int n = recv(client->socket, buffer->data + len, size - len, 0);
        if (n <= 0) break;
        len += n;
    }

    for(int x = 0; x < buffer->width; ++x) {
        for(int y = 0; y < buffer->height; ++y) {
            int* array = (int*) buffer->data;
            int* pixel = &array[x + y*buffer->width];

            unsigned int red =   (*pixel & 0x000000ff) >> 0;
            unsigned int green = (*pixel & 0x0000ff00) >> 8;
            unsigned int blue =  (*pixel & 0x00ff0000) >> 16;
        
            *pixel = 0xff000000 | (red << 16) | (green << 8)  | blue;
        }
    }
}


/*
void crfb_client_recv_raw_encoding(CRFBClient* client,) {
    unsigned int size = width * height * channels;
    unsigned int len = 0;

    while (len < size) {
        int n = recv(client->socket, buffer->data + len, size - len, 0);
        if (n <= 0) break;
        len += n;
    }

    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            int* array = (int*) buffer->data;
            int* pixel = &array[x + y*width];

            unsigned int red =   (*pixel & 0x000000ff) >> 0;
            unsigned int green = (*pixel & 0x0000ff00) >> 8;
            unsigned int blue =  (*pixel & 0x00ff0000) >> 16;
        
            *pixel = 0xff000000 | (red << 16) | (green << 8)  | blue;
        }
    }
}
*/

#endif