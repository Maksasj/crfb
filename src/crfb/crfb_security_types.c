#include "crfb_security_types.h"

void crfb_client_get_security_types(CRFBClient* client) {
    unsigned char count;
    if(recv(client->socket, &count, sizeof(unsigned char), MSG_WAITALL) <= 0)
        CRFB_LOG(CRFB_ERROR, "Failed to recv security type count");

    for(int i = 0; i < count; ++i) {
        unsigned char type;

        if(recv(client->socket, &type, sizeof(unsigned char), MSG_WAITALL) <= 0)
            CRFB_LOG(CRFB_ERROR, "Failed to recv security type count");
    }
}

char* crfb_stringify_security_type(CRFBSecurityType secType) {
    if     (secType == 0)   return "Invalid";
    else if(secType == 1)   return "No Authentication";
    else if(secType == 2)   return "VNC Authentication";
    else if(secType == 5)   return "RSA-AES Security Type";
    else if(secType == 6)   return "RSA-AES Unecrypted Security Type";
    else if(secType == 13)  return "RSA-AES Two-step Security Type";
    else if(secType == 16)  return "Tight Security Type";
    else if(secType == 19)  return "VenCrypt";
    else if(secType == 22)  return "xvp Authentication";
    else if(secType == 30)  return "Diffie-Hellman Authentication";
    else if(secType == 113) return "MSLogonII Authentication";
    else if(secType == 129) return "RSA-AES-256 Security Type";
    else if(secType == 130) return "RSA-AES-256 Unencrypted Security Type";
    else if(secType == 133) return "RSA-AES-256 Two-step Security Type";

    return "Unofficial security type";
} 
