#include "crfb_capability.h"

void crfb_client_recv_capabilities(CRFBClient* client, unsigned int capabilityCount) {
    for(int i = 0; i < capabilityCount; ++i) {
        CRFBCapability cap;

        int len = recv(client->socket, &cap.code, sizeof(cap.code), 0);
        if(len <= 0) CRFB_LOG(CRFB_ERROR, "Failed to recv server capability code");

        len = recv(client->socket, &cap.vendor, sizeof(cap.vendor), 0);
        if(len <= 0) CRFB_LOG(CRFB_ERROR, "Failed to recv server capabilities vendor");

        len = recv(client->socket, &cap.signature, sizeof(cap.signature), 0);
        if(len <= 0) CRFB_LOG(CRFB_ERROR, "Failed to recv server capabilities signature");

        crfb_int_to_little(&cap.code);
        CRFB_LOG(CRFB_INFO, "CRFBCapability (code: %d, vendor: %.4s, signature: %.8s)", cap.code, cap.vendor, cap.signature);
    }
}