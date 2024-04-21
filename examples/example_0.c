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

    crfb_client_send_set_encodings_message(client);

    crfb_client_send_framebuffer_update_request_message(client, 0, 0, 0, 1920, 1080);

    {
        char buffer[16] = { '\0' };
        int len = recv(client->socket, buffer, 16, 0);
        printf("Server sent: %d\n", len);
    }

    unsigned int width = serverInit.framebufferWidth;
    unsigned int height = serverInit.framebufferHeight;
    unsigned int channels = serverInit.pixelFormat.bitsPerPixel / 8;

    CRFBFramebuffer* buffer = crfb_create_frame_buffer(width, height, channels);

    printf("%d", channels);
    fflush(stdout);

    for(int frame = 0; ; ++frame) {
        crfb_client_send_framebuffer_update_request_message(client, 0, 0, 0, 1920, 1080);

        unsigned int size = width * height * channels;
        unsigned int len = 0;

        while (len < size) {
            int n = recv(client->socket, buffer->data + len, size - len, 0);
            
            if (n <= 0) break;
            
            len += n;
        }
    
        char fileName[50] = { '\0' };  
        sprintf(fileName, "sample_%d.png", frame);
        stbi_write_png("sample_.png", width, height, channels, buffer->data, width*channels);
    }

    crfb_free_frame_buffer(buffer);

    crfb_client_close(client);
    crfb_free_client(client);

    return 0;
}