#ifndef _CRFB_SECURITY_TYPES_H_
#define _CRFB_SECURITY_TYPES_H_

#include "crfb_backend.h"
#include "crfb_client.h"

typedef enum CRFBSecurityType {
    INVALID = 0,
    NO_AUTHENTICATION = 1,
    VNC_AUTHENTICATION = 2,
    RSA_AES_SECURITY_TYPE = 5,
    RSA_AES_UNEBCRYPTED_SECURITY_TYPE = 6,
    RSA_AES_TWO_STEP_SECURITY_TYPE = 13,
    TIGHT_SECURITY_TYPE = 16,
    VENCRYPT = 19,
    XVP_AUTHENTICATION = 22,
    DIFFIE_HELLMAN_AUTHENTICATION = 30,
    MSLOGONII_AUTHENTICATION = 113,
    RSA_AES_256_SECURITY_TYPE = 129,
    RSA_AES_256_UNEBCRYPTED_SECURITY_TYPE = 130,
    RSA_AES_256_TWO_STEP_SECURITY_TYPE = 133,
} CRFBSecurityType;

void crfb_client_get_security_types(CRFBClient* client);

char* crfb_stringify_security_type(CRFBSecurityType secType);

#endif