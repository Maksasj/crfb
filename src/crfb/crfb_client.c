#include "crfb_client.h"

CRFBClient* crfb_new_client() {
    #ifdef _WIN32
        WSADATA data;
    #endif 

    #ifdef _WIN32
        WSAStartup(MAKEWORD(2,2),&data);    
    #endif

    CRFBClient* client = (CRFBClient*) malloc(sizeof(CRFBClient));

    if ((client->socket = socket(AF_INET, SOCK_STREAM, 0))< 0){
        CRFB_LOG(CRFB_ERROR, "Failed to create CRFB client socket");
        exit(1);
    }
    
    int opt = 1;
    if (setsockopt(client->socket, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    return client;
}

void crfb_client_connect(CRFBClient* client, const char* ip, long port) {
    struct sockaddr_in servaddr; 

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET; // nurodomas protokolas (IP)
    servaddr.sin_port = htons(port); // nurodomas portas

    #ifdef _WIN32
        servaddr.sin_addr.s_addr = inet_addr(ip);
    #else
        if (inet_aton(ip, &servaddr.sin_addr) <= 0 ) {
            CRFB_LOG(CRFB_ERROR, "Invalid remote IP address");
            exit(1);
        }
    #endif   

    if (connect(client->socket, (struct sockaddr*)&servaddr,sizeof(servaddr))<0){
        CRFB_LOG(CRFB_ERROR, "Failed to connect()");
        exit(1);
    }
}

void crfb_client_close(CRFBClient* client) {
    close(client->socket);
}

void crfb_free_client(CRFBClient* client) {
    free(client);
}