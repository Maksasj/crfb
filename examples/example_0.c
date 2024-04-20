#include "crfp/crfp.h"

#define BUFFLEN 4096

void int_to_little(int* value) {
    *value = ((*value>>24)&0xff) | // move byte 3 to byte 0
        ((*value<<8)&0xff0000) | // move byte 1 to byte 2
        ((*value>>8)&0xff00) | // move byte 2 to byte 1
        ((*value<<24)&0xff000000); // byte 0 to byte 3
}

void short_to_little(short* value) {
    *value = ((*value & 0xFF00) >> 8) | ((*value & 0x00FF) << 8);
}

int main(int argc, char *argv[]){

    CRFPClient* client = crfp_new_client();

    crfp_client_connect(client, "192.168.1.103", 5900);
    
    {   // Handshake
        {
            char buffer[BUFFLEN] = { '\0' };
            int len = recv(client->socket, buffer, BUFFLEN, 0);
            printf("Server sent: %d '%s'\n", len, buffer);
        }

        char* version = "RFB 003.008\n"; 
        if(send(client->socket, version, strlen(version), 0) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }
    }

    printf("\n");

    {   // Gets security types
        char byte;
        int len = recv(client->socket, &byte, 1, 0);
        printf("Security types %x\n", byte);

        for(int i = 0; i < byte; ++i) {
            char sec;
            int len = recv(client->socket, &sec, 1, 0);
            printf("    Security type: %d\n", sec);

            if(sec == 0x00000000) printf("Invalid");
            else if(sec == 0x00000001) printf("No Authentication");
            else if(sec == 0x00000002) printf("VNC Authentication");
            else printf("Unofficial security type"); 

            printf("\n");                
        }
    }

    printf("\n");

    {   // Security hanshake
        char type = 0x00000001; 
        if(send(client->socket, &type, 1, 0) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }

        {
            unsigned int word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d '%d'\n", len, word);

            if(len == 0) {
                printf("LENGTH ZERO");
            }

            if(word == 0) {
                printf("    SecurityResult OK\n");
            } else if(word == 1) {
                printf("    SecurityResult Failed\n");
            } else {
                printf("    Unofficial SecurityResult type\n");
            }
        }
    }

    printf("\n");

    {   
        {   // ClientInit
            typedef struct ClientInit {
                char sharedFlag;
            } ClientInit;

            ClientInit clientInit;
            clientInit.sharedFlag = 0;

            if(send(client->socket, &clientInit, sizeof(ClientInit), 0) == -1) {
                fprintf(stderr,"ERROR failed to send packet to server\n");
            }

            printf("Send client init !\n");
        }
    
        {
            unsigned short word = 0;
            int len = recv(client->socket, &word, 2, 0);
            short_to_little(&word);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned short word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            short_to_little(&word);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned short word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            short_to_little(&word);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned short word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            short_to_little(&word);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned short word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            short_to_little(&word);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            unsigned char word = 0;
            int len = recv(client->socket, &word, sizeof(word), 0);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            char buffer[BUFFLEN] = { '\0' };
            int len = recv(client->socket, buffer, 2, 0);
            printf("Server sent: %d, '%d' '%d' '%d'\n", len, buffer[0], buffer[1], buffer[2]);
        }

        {
            unsigned int word = 0;
            int len = recv(client->socket, &word, 4, 0);
            int_to_little(&word);
            printf("Server sent: %d, '%d'\n", len, word);
        }

        {
            char buffer[BUFFLEN] = { '\0' };
            int len = recv(client->socket, buffer, BUFFLEN, 0);
            printf("Server sent: %d '%s'\n", len, buffer);
        }
    }

    printf("\n");

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
        req.width = 1920;
        req.height = 1080;

        if(send(client->socket, &req, sizeof(FramebufferUpdateRequest), 0) == -1) {
            fprintf(stderr,"ERROR failed to send packet to server\n");
        }

        {
            typedef struct FramebufferUpdate {
                unsigned char messageType;
                unsigned char padding;
                unsigned short numberOfRectangles;
            } FramebufferUpdate;
            
            FramebufferUpdate update;

            {
                int len = recv(client->socket, &update, sizeof(FramebufferUpdate), 0);
                printf("Server sent: %d\n", len);
            }

            short_to_little(&update.numberOfRectangles);

            printf("Message type: %d\n", update.messageType);
            printf("Number Of Rectangles %d\n", update.numberOfRectangles);

            for(int i = 0; i < 1; ++i) {
                printf("\n");
                typedef struct Rectangle {
                    unsigned short xPosition;
                    unsigned short yPosition;
                    unsigned short width;
                    unsigned short height;
                    unsigned int encodingType;
                } Rectangle;

                Rectangle rect;
                
                {
                    int len = recv(client->socket, &rect, sizeof(Rectangle), 0);
                    printf("Server sent: %d\n", len);

                    short_to_little(&rect.xPosition);
                    short_to_little(&rect.yPosition);
                    short_to_little(&rect.width);
                    short_to_little(&rect.height);

                    printf("xPosition: %d\n", rect.xPosition);
                    printf("yPosition: %d\n", rect.yPosition);
                    printf("width: %d\n", rect.width);
                    printf("height: %d\n", rect.height);
                    printf("encodingType: %d\n", rect.encodingType);
                }

                {
                    size_t size = 1920 * 1080 * 4;
                    int* buffer = (int*) malloc(size);

                    int len = recv(client->socket, buffer, size, 0);
                    printf("Server sent: %d\n", len);
                    if(len == 0 || len == -1) {
                        fprintf(stderr, "ERROR failed to recv packet from server\n");
                        break;
                    }

                    
 
                    free(buffer);
                }
            }
        }
    }

    crfp_client_close(client);
    crfp_free_client(client);

    return 0;
}