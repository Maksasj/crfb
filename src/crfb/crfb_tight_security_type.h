#ifndef _CRFB_TIGHT_SECURITY_TYPE_H_
#define _CRFB_TIGHT_SECURITY_TYPE_H_

#include "crfb_client.h"
#include "crfb_utils.h"

void crfb_client_run_tight_security_handshake(CRFBClient* client) {
    char type = TIGHT_SECURITY_TYPE; // CHOOSEN SECURITY TYPE TIGHT

    if(send(client->socket, &type, 1, 0) == -1)
        fprintf(stderr,"ERROR failed to send packet to server\n");

    // Tunnel count
    unsigned int tunnelCount = 0;
    int len = recv(client->socket, &tunnelCount, sizeof(tunnelCount), 0);
    crfb_uint_to_little(&tunnelCount);
    printf("Number of tunnels: '%d'\n", tunnelCount);

    // Authentication types
    unsigned int authTypeCount = 0;
    len = recv(client->socket, &authTypeCount, sizeof(authTypeCount), 0);
    crfb_uint_to_little(&authTypeCount);
    printf("Authentication types: '%d'\n", authTypeCount);
}

#endif