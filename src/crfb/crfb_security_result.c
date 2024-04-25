#include "crfb_security_result.h"

CRFBSecurityResult crfb_client_recv_security_result(CRFBClient* client) {
    CRFBSecurityResult result = 0;
    if(recv(client->socket, &result, sizeof(CRFBSecurityResult), MSG_WAITALL) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv security result");

    return result;
}