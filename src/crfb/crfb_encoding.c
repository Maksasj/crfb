#include "crfb_encoding.h"

void crfb_client_recv_raw_encoding(CRFBClient* client, CRFBFramebuffer* buffer, CRFBRectangle* rect) {
    unsigned short width = rect->width;
    unsigned short height = rect->height;

    unsigned int size = width * height * 4;
    unsigned int len = 0;

    void* pixels = malloc(size);

    while (len < size) {
        int n = recv(client->socket, pixels + len, size - len, 0);

        if (n <= 0) {
            CRFB_LOG(CRFB_WARNING, "Failed to recv raw encoding stream");
            break;
        }

        len += n;
    }
    
    // Handle color corrections, todo
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
    if(recv(client->socket, &encoding, sizeof(CopyRectEncoding), 0) <= 0) 
        CRFB_LOG(CRFB_ERROR, "Failed to recv copy rect encoding");

    crfb_ushort_to_little(&encoding.srcXPosition);
    crfb_ushort_to_little(&encoding.srcYPosition);

    unsigned short width = rect->width;
    unsigned short height = rect->height;
    unsigned int size = width * height * 4;

    void* pixels = malloc(size);

    for(int y = 0; y < height; ++y) {
        memcpy(
            pixels + ((y * width) * 4),
            buffer->data + (encoding.srcXPosition + (y + encoding.srcYPosition) * buffer->width) * 4,
            width * 4
        );
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
