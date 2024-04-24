#ifndef _CRFB_CLIENT_INIT_H_
#define _CRFB_CLIENT_INIT_H_

#include "crfb_backend.h"
#include "crfb_client.h"

typedef struct CRFBClientInit {
    char sharedFlag;
} CRFBClientInit;

void crfb_client_send_client_init(CRFBClient* client, unsigned char sharedFlag);

#endif