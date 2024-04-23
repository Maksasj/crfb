#include <SDL3/SDL.h>
#include <pthread.h>
#include "crfb/crfb.h"

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

	context.window = SDL_CreateWindow("CRFB Client [192.168.1.116:5900]", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

typedef struct AppContext {
	SDLContext* context;
	CRFBClient* client;
	CRFBFramebuffer* buffer;
	unsigned char exitFlag;
} AppContext;

void* handle_mouse_input(void *ptr) {
	AppContext* app = (AppContext*) ptr;
	CRFBClient* client = app->client;

	while(!app->exitFlag) {
		static float xPosition, yPosition;
		float newX, newY;
		unsigned int buttons = SDL_GetMouseState(&newX, &newY);

		if(xPosition != newX || yPosition != newY) {
			// printf("%x \n", buttons);
			crfb_client_send_pointer_event_message(client, buttons, xPosition, yPosition);
			xPosition = newX;
			yPosition = newY;
		}
	}

    return ptr;
}

void* screen_update_thread(void* ptr) {
	AppContext* app = (AppContext*) ptr;
	CRFBClient* client = app->client;
	CRFBFramebuffer* buffer = ((AppContext*) ptr)->buffer;

	while(!app->exitFlag) {
		crfb_client_send_framebuffer_update_request_message(client, 1, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		CRFBFramebufferUpdate update = crfb_client_recv_framebuffer_update_message(client);

		for(int i = 0; i < update.numberOfRectangles; ++i) {
			CRFBRectangle rect = crfb_client_recv_rectangle(client);

			if(rect.ecodingType == RAW_ENCODING) {
				crfb_client_recv_raw_encoding(client, buffer, &rect);
			} else if(rect.ecodingType == COPYRECT_ENCODING) {
				crfb_client_recv_copy_rect_encoding(client, buffer, &rect);
			} else if(rect.ecodingType == DESKTOP_SIZE_PSEUDO_ENCODING) {
				printf("Desktop size pseudo encoding\n");
			} else {
				printf("Unsuported rectangle encoding %x\n", rect.ecodingType);
				ptr;
			}
		}
	}

	return ptr;
}

int main(){
	CRFBClient* client = crfb_new_client();
    crfb_client_connect(client, "192.168.1.116", 5900);

    CRFBProtocolVersion version = crfb_client_recv_server_handshake(client);
    if(version != CRFB_003_008) 
		return 1;
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
        DESKTOP_SIZE_PSEUDO_ENCODING
    };
    crfb_client_send_set_encodings_message(client, encodings, 3);

    unsigned int width = serverInit.framebufferWidth;
    unsigned int height = serverInit.framebufferHeight;
    unsigned int channels = serverInit.pixelFormat.bitsPerPixel / 8;
    CRFBFramebuffer* buffer = crfb_create_frame_buffer(width, height, channels);

	SDLContext context = setup_sdl();

	AppContext app;
	app.context = &context;
	app.client = client;
	app.buffer = buffer;
	app.exitFlag = 0;

	pthread_t mouseThread, screenUpdateThread;
    pthread_create(&mouseThread, NULL, *handle_mouse_input, (void *) &app);
    pthread_create(&screenUpdateThread, NULL, *screen_update_thread, (void *) &app);

	while (!app.exitFlag) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if(event.type == SDL_EVENT_QUIT) {
				app.exitFlag = 1;
				break;
			} else if(event.type == SDL_EVENT_KEY_DOWN) {
				crfb_client_send_key_event_message(client, 1, event.key.keysym.sym);

				// "SDLK_TAB"
				// "SDLK_CAPSLOCK"
				// "SDLK_LSHIFT"
				// "SDLK_LCTRL"
				// "SDLK_LALT"
				// "SDLK_KP_ENTER"
				// "SDLK_BACKSPACE"
				if(event.key.keysym.sym == SDLK_LSHIFT) {
					printf("Poggers !\n");
				}

				break;
			} else if(event.type == SDL_EVENT_KEY_UP) {
				crfb_client_send_key_event_message(client, 0, event.key.keysym.sym);
				break;
			}
			
		}
		
		int pitch;
		SDL_RenderClear(context.renderer);
		SDL_LockTexture(context.texture, &window_rect, &pixels, &pitch);
		
		memcpy(pixels, buffer->data, (buffer->width * buffer->height) * 4);

		SDL_UnlockTexture(context.texture);
		SDL_RenderTexture(context.renderer, context.texture, &window_rect_f, &window_rect_f);
		SDL_RenderPresent(context.renderer);
	}

	crfb_free_frame_buffer(buffer);

	crfb_client_close(client);
	crfb_free_client(client);

    SDL_DestroyTexture(context.texture);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);

    return 0;
}