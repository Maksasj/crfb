#ifndef _CRFB_CLIENT_H_
#define _CRFB_CLIENT_H_

#include "crfb_backend.h"

typedef struct CRFBClient {
    int socket;
} CRFBClient;

CRFBClient* crfb_new_client();

void crfb_client_connect(CRFBClient* client, const char* ip, long port);

void crfb_client_close(CRFBClient* client);

void crfb_free_client(CRFBClient* client);
#endif