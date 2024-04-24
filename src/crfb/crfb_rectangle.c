#include "crfb_rectangle.h"

void crfb_client_recv_rectangle(CRFBClient* client, CRFBRectangle* rectangle) {
    if(recv(client->socket, rectangle, sizeof(CRFBRectangle), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv rectangle");

    crfb_ushort_to_little(&rectangle->xPosition);
    crfb_ushort_to_little(&rectangle->yPosition);
    crfb_ushort_to_little(&rectangle->width);
    crfb_ushort_to_little(&rectangle->height);
    crfb_int_to_little(&rectangle->ecodingType);
}

CRFBResult crfb_client_valid_rectangle(CRFBRectangle* rectangle, unsigned int width, unsigned int height) {
    if(rectangle->xPosition > width)
        return CRFB_FAILED;

    if(rectangle->yPosition > height)
        return CRFB_FAILED;

    if(rectangle->width > width)
        return CRFB_FAILED;

    if(rectangle->height > height)
        return CRFB_FAILED;

    if(rectangle->width == 0)
        return CRFB_FAILED;

    if(rectangle->height == 0)
        return CRFB_FAILED;

    return CRFB_OK;
}