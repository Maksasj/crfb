#include "crfb_client_init.h"

void crfb_client_send_client_init(CRFBClient* client, unsigned char sharedFlag) {
    CRFBClientInit clientInit;
    clientInit.sharedFlag = sharedFlag;

    if(send(client->socket, &clientInit, sizeof(CRFBClientInit), 0) == -1) {
        CRFB_LOG(CRFB_ERROR, "Failed to send packet to server");
    }
}
