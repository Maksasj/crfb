#include "crfp/crfp.h"

#define BUFFLEN 4096

int main(int argc, char *argv[]){

    CRFPClient* client = crfp_new_client();

    crfp_client_connect(client, "192.168.1.103", 5900);
    
    {
        char buffer[BUFFLEN] = { '\0' };
        int len = recv(client->socket, buffer, BUFFLEN, 0);
        printf("Server sent: %d '%s'\n", len, buffer);
    }

    char* version = "RFB 003.008\n"; 
    if(send(client->socket, version, strlen(version), 0) == -1) {
        fprintf(stderr,"ERROR failed to send packet to server\n");
    }

    {
        char byte;
        int len = recv(client->socket, &byte, 1, 0);
        printf("Security types %d\n", byte);

        for(int i = 0; i < byte; ++i) {
            char sec;
            int len = recv(client->socket, &sec, 1, 0);
            printf("    Security type: %d\n", sec);
        }
    }

    
    crfp_client_close(client);
    crfp_free_client(client);

    return 0;
}