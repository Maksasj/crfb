#include "crfp/crfp.h"

#define BUFFLEN 1024

int main(int argc, char *argv[]){

    CRFPClient* client = crfp_new_client();

    crfp_client_connect(client, "192.168.1.103", 5900);
    
    char buffer[BUFFLEN] = { '\0' };
    recv(client->socket, buffer, BUFFLEN, 0);
    printf("Server sent: %s\n", buffer);

    crfp_client_close(client);
    crfp_free_client(client);

    return 0;
}