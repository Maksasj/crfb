#ifndef _CRFB_TIGHT_SECURITY_TYPE_H_
#define _CRFB_TIGHT_SECURITY_TYPE_H_

#include "crfb_logger.h"
#include "crfb_client.h"
#include "crfb_utils.h"
#include "crfb_security_types.h"

void crfb_client_run_none_security_handshake(CRFBClient* client);
void crfb_client_run_tight_security_handshake(CRFBClient* client);

#endif