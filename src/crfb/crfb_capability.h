#ifndef _CRFB_CAPABILITY_H_
#define _CRFB_CAPABILITY_H_

#include "crfb_server_init.h"

typedef struct CRFBCapability {
    signed int code;
    char vendor[4]; 
    char signature[8]; 
} CRFBCapability;

void crfb_client_recv_capabilities(CRFBClient* client, unsigned int capabilityCount);

#endif