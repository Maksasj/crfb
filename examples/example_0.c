#include "crfb/crfb.h"

#define BUFFLEN 4096

#include "stb_image_write.h"

int main(int argc, char *argv[]){
    CRFBClient* client = crfb_new_client();
    crfb_client_connect(client, "192.168.1.103", 5900);

    CRFBProtocolVersion version = crfb_client_recv_server_handshake(client);
    if(version != CRFB_003_008) return 1;
    crfb_client_send_handshake(client, CRFB_003_008);

    crfb_client_get_security_types(client);
    crfb_client_run_tight_security_handshake(client);

    CRFBSecurityResult result = crfb_client_recv_security_result(client);
        if(result != CRFB_SECURITY_RESULT_OK)
            return 1;

    crfb_client_send_client_init(client, 0);

    CRFBServerInit serverInit = crfb_client_recv_server_init(client);
    crfb_printify_server_init(serverInit);

    CRFBTightServerInitExt ext = crfb_client_recv_tight_server_init_extension(client);
    crfb_client_recv_capabilities(client, ext.numberOfServerMessages);
    crfb_client_recv_capabilities(client, ext.numberOfClientMessages);
    crfb_client_recv_capabilities(client, ext.numberOfEncodings);

    CRFBEncoding encodings[] = {
        RAW_ENCODING,
        0xffffff21, 
    };
    crfb_client_send_set_encodings_message(client, encodings, 8);

    unsigned int width = serverInit.framebufferWidth;
    unsigned int height = serverInit.framebufferHeight;
    unsigned int channels = serverInit.pixelFormat.bitsPerPixel / 8;

    CRFBFramebuffer* buffer = crfb_create_frame_buffer(width, height, channels);

    for(int frame = 0; ; ++frame) {
        crfb_client_send_framebuffer_update_request_message(client, 0, 0, 0, 1920, 1080);

        CRFBFramebufferUpdate update = crfb_client_recv_framebuffer_update_message(client);

        for(int i = 0; i < update.numberOfRectangles; ++i) {
            CRFBRectangle rect = crfb_client_recv_rectangle(client);

            printf("%d, %d, %d, %d, %d\n", rect.xPosition, rect.yPosition, rect.width, rect.height, rect.ecodingType);

            if(rect.ecodingType == RAW_ENCODING) {
                crfb_client_recv_raw_encoding(client, buffer);

                char fileName[50] = { '\0' };  
                sprintf(fileName, "sample_%d.png", frame);
                stbi_write_png("sample.png", width, height, channels, buffer->data, width*channels);
            } else if(rect.ecodingType == DESKTOP_SIZE_PSEUDO_ENCODING) {
                printf("Desktop size pseudo encoding\n");
            } else {
                printf("Unsuported rectangle encoding %x\n", rect.ecodingType);
                // return 1;
            }
        }
    }

    crfb_free_frame_buffer(buffer);

    crfb_client_close(client);
    crfb_free_client(client);

    return 0;
}