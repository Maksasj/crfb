#ifndef _CRFB_CLIENT_TO_SERVER_MESSAGES_H_
#define _CRFB_CLIENT_TO_SERVER_MESSAGES_H_

#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <X11/Xlib.h>

#include "crfb_backend.h"
#include "crfb_client.h"

#include "crfb_encoding.h"

typedef enum CRFBCLientToServerMessage {
    SET_PIXEL_FORMAT = 0,               // [ ]
    SET_ENCODINGS = 2,                  // [x]
    FRAMEBUFFER_UPDATE_REQUEST = 3,     // [x]
    KEY_EVENT = 4,                      // [x]
    POINTER_EVENT = 5,                  // [ ]
    CLIENT_CUT_TEXT = 6                 // [ ]
} CRFBCLientToServerMessage;

void crfb_client_send_set_encodings_message(
    CRFBClient* client, 
    CRFBEncoding* encodings, 
    unsigned long count);

void crfb_client_send_framebuffer_update_request_message(
    CRFBClient* client, 
    unsigned char incremental, 
    unsigned short xPosition,
    unsigned short yPosition,
    unsigned short width,
    unsigned short height);

void crfb_client_send_key_event_message(
    CRFBClient* client, 
    unsigned char downFlag, 
    unsigned int key);

void crfb_client_send_pointer_event_message(
    CRFBClient* client, 
    unsigned char buttonMask, 
    unsigned short xPosition, 
    unsigned short yPosition);

#endif