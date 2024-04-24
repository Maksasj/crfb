#include "crfb_tight_security_type.h"

void crfb_client_run_none_security_handshake(CRFBClient* client) {
    CRFB_LOG(CRFB_INFO, "Running None security handshake");

    unsigned char type = NO_AUTHENTICATION;

    if(send(client->socket, &type, 1, 0) == -1)
        CRFB_LOG(CRFB_ERROR, "Failed to send none security type");
}

void crfb_client_run_tight_security_handshake(CRFBClient* client) {
    CRFB_LOG(CRFB_INFO, "Running Tight security handshake");

    unsigned char type = TIGHT_SECURITY_TYPE;

    if(send(client->socket, &type, 1, 0) == -1)
        CRFB_LOG(CRFB_ERROR, "Failed to send tight security type");

    unsigned int tunnelCount = 0;
    if(recv(client->socket, &tunnelCount, sizeof(tunnelCount), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv tight security handshake tunnel count");

    unsigned int authTypeCount = 0;
    if(recv(client->socket, &authTypeCount, sizeof(authTypeCount), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv tight security handshake authentication type count");

    crfb_uint_to_little(&tunnelCount);
    crfb_uint_to_little(&authTypeCount);

    CRFB_LOG(CRFB_INFO, "TightTight security tunnel count %d", tunnelCount);
    CRFB_LOG(CRFB_INFO, "TightTight security authentication type count %d", authTypeCount);
}
