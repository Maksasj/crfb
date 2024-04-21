#ifndef _CRFB_CAPABILITY_H_
#define _CRFB_CAPABILITY_H_

#include "crfb_server_init.h"

typedef struct CRFBCapability {
                signed int code;
                char vendor[4]; 
                char signature[8]; 
} CRFBCapability;


void crfb_client_recv_capabilities(CRFBClient* client, unsigned int capabilityCount) {
    for(int i = 0; i < capabilityCount; ++i) {
        CRFBCapability cap;

        int len = recv(client->socket, &cap.code, sizeof(cap.code), 0);
        len = recv(client->socket, &cap.vendor, sizeof(cap.vendor), 0);
        len = recv(client->socket, &cap.signature, sizeof(cap.signature), 0);

        crfb_int_to_little(&cap.code);

        printf("    CRFBCapability\n");
        printf("        Code: %d\n", cap.code);
        printf("        Vendor: %.4s\n", cap.vendor);
        printf("        Signature: %.8s\n", cap.signature);
    }
}

#endif