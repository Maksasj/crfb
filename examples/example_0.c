#include "crfb/crfb.h"

#define BUFFLEN 4096

#include "stb_image_write.h"

int main(int argc, char *argv[]){
    CRFBClient* client = crfb_new_client();

    crfb_client_connect(client, "192.168.1.103", 5900);

    CRFBProtocolVersion version = crfb_client_recv_server_handshake(client);

    if(version != CRFB_003_008)
        return 1;

    crfb_client_send_handshake(client, CRFB_003_008);

    {   // Gets security types
        char byte;
        int len = recv(client->socket, &byte, 1, 0);
        printf("Security types %x\n", byte);

        for(int i = 0; i < byte; ++i) {
            char sec;
            int len = recv(client->socket, &sec, 1, 0);
            printf("    Security type: %d\n", sec);

            printf("%s", crfb_stringify_security_type(sec));

            printf("\n");                
        }
    }

    printf("\n");
    
    { // Tight Security Type Security handskae
        char type = 16; 
        if(send(client->socket, &type, 1, 0) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }

        {   // Tunnel count
            unsigned int word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            crfb_int_to_little(&word);
            printf("Number of tunnels: '%d'\n", word);
        }

        {   // Authentication types
            unsigned int word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            crfb_int_to_little(&word);
            printf("Authentication types: '%d'\n", word);
        }

        {   // Security result
            unsigned int word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d '%d'\n", len, word);

            if(len == 0) printf("LENGTH ZERO");

            if(word == 0) {
                printf("    SecurityResult OK\n");
            } else if(word == 1) {
                printf("    SecurityResult Failed\n");
            } else {
                printf("    Unofficial SecurityResult type\n");
            }
        }
    }

    crfb_client_send_client_init(client, 0);

    CRFBServerInit serverInit = crfb_client_recv_server_init(client);
    crfb_printify_server_init(serverInit);

    CRFBTightServerInitExt ext = crfb_client_recv_tight_server_init_extension(client);
    
    crfb_client_recv_capabilities(client, ext.numberOfServerMessages);
    crfb_client_recv_capabilities(client, ext.numberOfClientMessages);
    crfb_client_recv_capabilities(client, ext.numberOfEncodings);

    {
        // Magic packet, probably set encodings
        char message[40] = {
            0x02, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 
            0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 
            0xff, 0xff, 0xff, 0x21, 0xff, 0xff, 0xff, 0x20, 
            0xff, 0xff, 0xff, 0x18, 0xff, 0xff, 0xff, 0x11,
        }; 

        if(send(client->socket, &message, 40, 0) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }
    }

    /*
    {   // This is tigth packet
        // Mistery packet
        char message[4] = {
            0xfc, 0x00, 0x02, 0x01
        }; 

        if(send(client->socket, &message, 4, MSG_DONTWAIT) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }
    }
    */

    {
        typedef struct FramebufferUpdateRequest {
            unsigned char messageType;
            unsigned char incremental;

            unsigned short xPosition;
            unsigned short yPosition;
            unsigned short width;
            unsigned short height;
        } FramebufferUpdateRequest;

        FramebufferUpdateRequest req;
        req.messageType = 3;
        req.incremental = 0; // Lost

        req.xPosition = 0;
        req.yPosition = 0;
        req.width = 4320;
        req.height = 1350;

        crfb_short_to_little(&req.xPosition);
        crfb_short_to_little(&req.yPosition);
        crfb_short_to_little(&req.width);
        crfb_short_to_little(&req.height);

        if(send(client->socket, &req, sizeof(FramebufferUpdateRequest), MSG_DONTWAIT) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }

        printf("Send framebuffer update request !\n");

        {
            char buffer[16] = { '\0' };
            int len = recv(client->socket, buffer, 16, 0);
            printf("Server sent: %d\n", len);
        }
    }

    for(int frame = 0; ; ++frame) {
        typedef struct FramebufferUpdateRequest {
            unsigned char messageType;
            unsigned char incremental;

            unsigned short xPosition;
            unsigned short yPosition;
            unsigned short width;
            unsigned short height;
        } FramebufferUpdateRequest;

        FramebufferUpdateRequest req;
        req.messageType = 3;
        req.incremental = 0; // Lost

        req.xPosition = 0;
        req.yPosition = 0;
        req.width = 4320;
        req.height = 1350;

        crfb_short_to_little(&req.xPosition);
        crfb_short_to_little(&req.yPosition);
        crfb_short_to_little(&req.width);
        crfb_short_to_little(&req.height);

        if(send(client->socket, &req, sizeof(FramebufferUpdateRequest), MSG_DONTWAIT) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }

        printf("Send framebuffer update request !\n");

       {
            size_t size = 4320 * 1350 * 4;
                unsigned char* buffer = (char*) malloc(size);

                size_t len = 0;
                while (len < size) {
                    ssize_t n = recv(client->socket, buffer + len, size - len, 0);
                    if (n <= 0) {
                        // error handling here
                        break;
                        printf("Errpr !\n");
                    }
                    len += n;
                }

                printf("%ld\n", len);

                for(int x = 0; x < 4320; ++x) {
                    for(int y = 0; y < 1350; ++y) {
                        printf("%x ", *(buffer + 4320 * y + x));
                    }
                    printf("\n");
                }   


                char fileName[50] = { '\0' };  
                sprintf(fileName, "sample_%d.png", frame);
                stbi_write_png(fileName, 4320, 1350, 4, buffer, 4320*4);

                free(buffer);        
        }
    }

    crfb_client_close(client);
    crfb_free_client(client);

    return 0;
}