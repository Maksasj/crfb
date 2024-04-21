#ifndef _CRFB_SECURITY_TYPES_H_
#define _CRFB_SECURITY_TYPES_H_

#include "crfb_backend.h"

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

#endif