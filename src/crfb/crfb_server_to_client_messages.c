#include "crfb_server_to_client_messages.h"

void crfb_client_recv_framebuffer_update_message(CRFBClient* client, CRFBFramebufferUpdate* packet) {
    if(recv(client->socket, packet, sizeof(CRFBFramebufferUpdate), 0) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv framebuffer update message");

    crfb_ushort_to_little(&packet->numberOfRectangles);
}
