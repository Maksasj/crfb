#ifndef _CRFB_VERSION_H_
#define _CRFB_VERSION_H_

#include "crfb_backend.h"
#include "crfb_client.h"

typedef enum CRFBProtocolVersion {
    CRFB_003_003,
    CRFB_003_007,
    CRFB_003_008,
    CRFB_LATEST = CRFB_003_008,

    CRFB_UNSUPPORTED,
} CRFBProtocolVersion;

CRFBProtocolVersion crfb_client_recv_server_handshake(CRFBClient* client);

int crfb_client_send_handshake(CRFBClient* client, CRFBProtocolVersion version);

#endif