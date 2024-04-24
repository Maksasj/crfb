#ifndef _CRFB_CLIENT_INIT_H_
#define _CRFB_CLIENT_INIT_H_

#include "crfb_backend.h"
#include "crfb_client.h"

typedef struct CRFBClientInit {
    char sharedFlag;
} CRFBClientInit;

void crfb_client_send_client_init(CRFBClient* client, unsigned char sharedFlag) {
    CRFBClientInit clientInit;
    clientInit.sharedFlag = sharedFlag;

    if(send(client->socket, &clientInit, sizeof(CRFBClientInit), 0) == -1) {
        CRFB_LOG(CRFB_ERROR, "Failed to send packet to server");
    }
}

#endif