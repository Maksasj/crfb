#include "crfb_version.h"

CRFBProtocolVersion crfb_client_recv_server_handshake(CRFBClient* client) {
    char buffer[13] = { '\0' };

    if(recv(client->socket, buffer, 12, 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv server version");

    if(strcmp(buffer, "RFB 003.003\n") == 0)
        return CRFB_003_003;
    else if(strcmp(buffer, "RFB 003.007\n") == 0)
        return CRFB_003_007;
    else if(strcmp(buffer, "RFB 003.008\n") == 0)
        return CRFB_003_008;

    return CRFB_UNSUPPORTED;
}

int crfb_client_send_handshake(CRFBClient* client, CRFBProtocolVersion version) {
    char* buffer;

    if(version == CRFB_003_003) {
        buffer = "RFB 003.003\n";
    } else if(version == CRFB_003_007) {
        buffer = "RFB 003.007\n";
    } else if(version == CRFB_003_008) {
        buffer = "RFB 003.008\n";
    }

    if(send(client->socket, buffer, 12, 0) == -1)
        fprintf(stderr,"ERROR failed to send packet to server\n");
}
