#ifndef _CRFB_RECTANGLE_H_
#define _CRFB_RECTANGLE_H_

#include "crfb_logger.h"
#include "crfb_client.h"
#include "crfb_utils.h"
#include "crfb_result.h"

typedef struct CRFBRectangle {
    unsigned short xPosition;
    unsigned short yPosition;
    unsigned short width;
    unsigned short height;
    int ecodingType;// Todo
} CRFBRectangle;

void crfb_client_recv_rectangle(CRFBClient* client, CRFBRectangle* rectangle);

CRFBResult crfb_client_valid_rectangle(CRFBRectangle* rectangle, unsigned int, unsigned int);

#endif