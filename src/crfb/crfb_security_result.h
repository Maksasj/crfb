#ifndef _CRFB_SECURITY_RESULT_H_
#define _CRFB_SECURITY_RESULT_H_

#include "crfb_logger.h"
#include "crfb_client.h"

typedef unsigned int CRFBSecurityResult;

#define CRFB_SECURITY_RESULT_OK             ((CRFBSecurityResult) 0)
#define CRFB_SECURITY_RESULT_FAILED         ((CRFBSecurityResult) 1)

CRFBSecurityResult crfb_client_recv_security_result(CRFBClient* client);

#endif