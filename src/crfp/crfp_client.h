#ifndef _CRFP_CLIENT_H_
#define _CRFP_CLIENT_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>

typedef struct CRFPClient {
    int socket;
} CRFPClient;

CRFPClient* crfp_new_client() {
    CRFPClient* client = (CRFPClient*) malloc(sizeof(CRFPClient));

    if ((client->socket = socket(AF_INET, SOCK_STREAM,0))< 0){
        fprintf(stderr, "ERROR cannot crfp client socket.\n");
        exit(1);
    }

    return client;
}

void crfp_client_connect(CRFPClient* client, const char* ip, long port) {
    struct sockaddr_in servaddr; 

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET; // nurodomas protokolas (IP)
    servaddr.sin_port = htons(port); // nurodomas portas

    if (inet_aton(ip, &servaddr.sin_addr) <= 0 ) {
        fprintf(stderr,"ERROR #3: Invalid remote IP address.\n");
        exit(1);
    }

    if (connect(client->socket, (struct sockaddr*)&servaddr,sizeof(servaddr))<0){
        fprintf(stderr,"ERROR #4: error in connect().\n");
        exit(1);
    }
}

void crfp_client_close(CRFPClient* client) {
    close(client->socket);
}

void crfp_free_client(CRFPClient* client) {
    free(client);
}

#endif