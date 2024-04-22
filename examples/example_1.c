#include <SDL3/SDL.h>
#include "crfb/crfb.h"

#define BUFFLEN 4096

#define SERVER_WIDTH 	(800)
#define SERVER_HEIGHT 	(600)

#define RATIO 			1

#define WINDOW_WIDTH 	(SERVER_WIDTH / RATIO)
#define WINDOW_HEIGHT 	(SERVER_HEIGHT / RATIO)

const SDL_FRect window_rect_f = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};
const SDL_Rect window_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

typedef struct SDLContext {
	SDL_Window* window;
    SDL_Renderer* renderer;
	SDL_Texture* texture;
} SDLContext;
void *pixels;

SDLContext setup_sdl() {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDLContext context;

	context.window = SDL_CreateWindow("CRFB Client", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	context.renderer = SDL_CreateRenderer(context.window, NULL, 0);

	context.texture = SDL_CreateTexture(
		context.renderer, 
		SDL_PIXELFORMAT_ABGR8888, 
		SDL_TEXTUREACCESS_STREAMING, 
		WINDOW_WIDTH, 
		WINDOW_HEIGHT);

    int pitch;
    SDL_LockTexture(context.texture, &window_rect, &pixels, &pitch);

	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

	return context;
}

int main(){
	SDLContext context = setup_sdl();

	CRFBClient* client = crfb_new_client();
    crfb_client_connect(client, "192.168.1.116", 5900);

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
		COPYRECT_ENCODING,
        DESKTOP_SIZE_PSEUDO_ENCODING, 
    };
    crfb_client_send_set_encodings_message(client, encodings, 3);

    unsigned int width = serverInit.framebufferWidth;
    unsigned int height = serverInit.framebufferHeight;
    unsigned int channels = serverInit.pixelFormat.bitsPerPixel / 8;

    CRFBFramebuffer* buffer = crfb_create_frame_buffer(width, height, channels);

	unsigned long long frame = 0;

	unsigned char exit = 0;
	while (!exit) {
		++frame;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT: {
					exit = 1;
					break;
				}

				
			}

			if(frame % 5 == 0) {
				float xPosition, yPosition;
				unsigned int buttons = SDL_GetMouseState(&xPosition, &yPosition);
				crfb_client_send_pointer_event_message(client, 0, xPosition, yPosition);
			}

			// crfb_client_send_key_event_message(client, 1, XK_Up);
			// crfb_client_send_key_event_message(client, 0, XK_Up);

			if(frame % 30 == 0) {
				crfb_client_send_framebuffer_update_request_message(client, 0, 0, 0, SERVER_WIDTH, SERVER_HEIGHT);
				CRFBFramebufferUpdate update = crfb_client_recv_framebuffer_update_message(client);
				for(int i = 0; i < update.numberOfRectangles; ++i) {
					CRFBRectangle rect = crfb_client_recv_rectangle(client);

					printf("%d, %d, %d, %d, %d\n", rect.xPosition, rect.yPosition, rect.width, rect.height, rect.ecodingType);

					if(rect.ecodingType == RAW_ENCODING) {
						crfb_client_recv_raw_encoding(client, buffer);

						SDL_RenderClear(context.renderer);
						int pitch;
						SDL_LockTexture(context.texture, &window_rect, &pixels, &pitch);

						for(int x = 0; x < WINDOW_WIDTH; ++x) {
							for(int y = 0; y < WINDOW_HEIGHT; ++y) {
								int* array = (int*) buffer->data;
								int pixel = array[x * RATIO+ y*buffer->width * RATIO];

								((unsigned int*) pixels)[x + y*WINDOW_WIDTH] = pixel;
							}
						}

						SDL_UnlockTexture(context.texture);

						SDL_RenderTexture(context.renderer, context.texture, &window_rect_f, &window_rect_f);
						SDL_RenderPresent(context.renderer);
					} else if(rect.ecodingType == COPYRECT_ENCODING) {
						printf("Copyrect encoding !\n");
						return 1;
					} else if(rect.ecodingType == DESKTOP_SIZE_PSEUDO_ENCODING) {
						printf("Desktop size pseudo encoding\n");
					} else {
						printf("Unsuported rectangle encoding %x\n", rect.ecodingType);
						// return 1;
					}
				}
			}
		}
	}

    SDL_DestroyTexture(context.texture);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);

    return 0;
}