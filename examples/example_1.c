#include <SDL3/SDL.h>
#include <pthread.h>
// #include <sys/select.h>

#include "crfb/crfb.h"

#define SERVER_WIDTH 	(800)
#define SERVER_HEIGHT 	(600)

#define RATIO 			1

#define WINDOW_WIDTH 	(SERVER_WIDTH / RATIO)
#define WINDOW_HEIGHT 	(SERVER_HEIGHT / RATIO)

const SDL_FRect window_rect_f = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};
const SDL_Rect window_rect = 	{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

typedef struct SDLContext {
	SDL_Window* window;
    SDL_Renderer* renderer;
	SDL_Texture* texture;
} SDLContext;

typedef struct AppContext {
	SDLContext* context;
	void *pixels;

	CRFBClient* client;
	CRFBFramebuffer* buffer;
	unsigned char exitFlag;
} AppContext;

CRFBResult setup_sdl(AppContext* app) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDLContext* context = (SDLContext*) malloc(sizeof(SDLContext));

	context->window = SDL_CreateWindow("CRFB Client [192.168.1.116:5900]", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if(context->window == NULL) {
		CRFB_LOG(CRFB_ERROR, "Failed to create SDL window %s\n", SDL_GetError());
		return CRFB_FAILED;
	}

	context->renderer = SDL_CreateRenderer(context->window, NULL, SDL_RENDERER_ACCELERATED);
	if(context->renderer == NULL) {
		CRFB_LOG(CRFB_ERROR, "Failed to create SDL renderer %s\n", SDL_GetError());
		return CRFB_FAILED;
	}

	context->texture = SDL_CreateTexture(
		context->renderer, 
		SDL_PIXELFORMAT_ABGR8888, 
		SDL_TEXTUREACCESS_STREAMING, 
		WINDOW_WIDTH, 
		WINDOW_HEIGHT);

	if(context->texture == NULL) {
		CRFB_LOG(CRFB_ERROR, "Failed to create SDL window\n");
		return CRFB_FAILED;
	}

    int pitch;

    SDL_LockTexture(context->texture, &window_rect, &app->pixels, &pitch);
	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

	app->context = context;

	return CRFB_OK;
}

void* handle_mouse_input(void *ptr) {
	AppContext* app = (AppContext*) ptr;
	CRFBClient* client = app->client;

	while(!app->exitFlag) {
		static float xPosition, yPosition;
		float newX, newY;
		unsigned int buttons = SDL_GetMouseState(&newX, &newY);

		if((xPosition != newX || yPosition != newY) || buttons != 0) {
			crfb_client_send_pointer_event_message(client, buttons, xPosition, yPosition);
			xPosition = newX;
			yPosition = newY;
		}
	}

    return ptr;
}

void empty_recv_buffer(int sockfd) {
	CRFB_LOG(CRFB_INFO, "Emptying recv buffer, resetting stream");

    char buffer[1024];
    int bytes = 0;

    fd_set read_fds;
    struct timeval tv;
    int retval;

    // Watch sockfd to see when it has input
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

    // Wait up to one second
    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    do {
        retval = select(sockfd + 1, &read_fds, NULL, NULL, &tv);
        if (retval == -1) {
            perror("select()");
            return;
        } else if (retval) {
            bytes = recv(sockfd, buffer, sizeof(buffer), 0);
        } else {
            // No data within one second
            printf("No data within one second.\n");
            return;
        }
    } while (bytes > 0);
}

void* screen_update_thread(void* ptr) {
	AppContext* app = (AppContext*) ptr;
	CRFBClient* client = app->client;
	CRFBFramebuffer* buffer = ((AppContext*) ptr)->buffer;

	while(!app->exitFlag) {
		crfb_client_send_framebuffer_update_request_message(client, 1, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		CRFBFramebufferUpdate update;
		crfb_client_recv_framebuffer_update_message(client, &update);

		if(update.messageType != FRAMEBUFFER_UPDATE) {
			CRFB_LOG(CRFB_ERROR, "Message type is not a FRAMEBUFFER_UPDATE %d", update.messageType);
			app->exitFlag = 1;
			break;
		}

		CRFB_LOG(CRFB_INFO, "Receiving %d rectangles", update.numberOfRectangles);
		for(int i = 0; i < update.numberOfRectangles; ++i) {
			CRFBRectangle rect;
			crfb_client_recv_rectangle(client, &rect);

			if(crfb_client_valid_rectangle(&rect, WINDOW_WIDTH, WINDOW_HEIGHT) != CRFB_OK) {
				CRFB_LOG(CRFB_ERROR, "Received invalid rectangle [%d] %d %d %d %d %d", i, rect.xPosition, rect.yPosition, rect.width, rect.height, rect.ecodingType);
				empty_recv_buffer(client->socket);
				break;
				// continue;
			}

			CRFB_LOG(CRFB_INFO, "Received rectangle [%d] %d %d %d %d %d", i, rect.xPosition, rect.yPosition, rect.width, rect.height, rect.ecodingType);

			if(rect.ecodingType == RAW_ENCODING) {
				crfb_client_recv_raw_encoding(client, buffer, &rect);
			} else if(rect.ecodingType == COPYRECT_ENCODING) {
				crfb_client_recv_copy_rect_encoding(client, buffer, &rect);
			} else if(rect.ecodingType == DESKTOP_SIZE_PSEUDO_ENCODING) {
				CRFB_LOG(CRFB_WARNING, "Desktop size pseudo encoding");
			} else {
				CRFB_LOG(CRFB_ERROR, "Unsuported rectangle encoding [%d] %d %d %d %d %d", i, rect.xPosition, rect.yPosition, rect.width, rect.height, rect.ecodingType);
			}
		}
	}

	return ptr;
}

CRFBResult setup_crfb(AppContext* app) {
	char adress[] = "26.250.187.108";
	unsigned int port = 5900;

	CRFBClient* client = crfb_new_client();
	CRFB_LOG(CRFB_INFO, "Started CRFB client");

    crfb_client_connect(client, adress, port);
	CRFB_LOG(CRFB_INFO, "Connected to %s:%d server", adress, port);

    CRFBProtocolVersion version = crfb_client_recv_server_handshake(client);
    if(version != CRFB_003_008) 
		return CRFB_FAILED;

    crfb_client_send_handshake(client, CRFB_003_008);

    crfb_client_get_security_types(client);
	crfb_client_run_tight_security_handshake(client);
	// crfb_client_run_none_security_handshake(client);

    CRFBSecurityResult result = crfb_client_recv_security_result(client);
	if(result != CRFB_SECURITY_RESULT_OK)
		return CRFB_FAILED;

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

	app->client = client;
	app->buffer = crfb_create_frame_buffer(width, height, channels);

	return CRFB_OK;
}

void handle_sdl_events(AppContext* app) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if(event.type == SDL_EVENT_QUIT) {
			app->exitFlag = 1;
			break;
		} else if(event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			unsigned int key = event.key.keysym.sym;

			if(key == SDLK_TAB) key = 0xff09; //XK_Tab;
            if(key == SDLK_CAPSLOCK) key = 0xffe5;// XK_Caps_Lock;

            if(key == SDLK_LSHIFT) key = 0xffe1; //XK_Shift_L;
            if(key == SDLK_RSHIFT) key = 0xffe2; // XK_Shift_R;

            if(key == SDLK_LCTRL) key = 0xffe3; // XK_Control_L;
            if(key == SDLK_RCTRL) key = 0xffe4; // XK_Control_R;

            if(key == SDLK_LALT) key = 0xffe9; // XK_Alt_L;
            if(key == SDLK_RALT) key = 0xffea; // XK_Alt_R;

            if(key == SDLK_RETURN) key = 0xff8d; // XK_KP_Enter;

            if(key == SDLK_BACKSPACE) key = 0xff08; // XK_BackSpace;

			if(event.type == SDL_EVENT_KEY_DOWN)
				crfb_client_send_key_event_message(app->client, 1, key);

			if(event.type == SDL_EVENT_KEY_UP)
				crfb_client_send_key_event_message(app->client, 0, key);

			break;
		}
	}
}

int main(){
	AppContext app;
	app.exitFlag = 0;

	if(setup_crfb(&app) != CRFB_OK) {
		CRFB_LOG(CRFB_ERROR, "Failed to start crfb");
		return 1;
	}

	if(setup_sdl(&app) != CRFB_OK) {
		CRFB_LOG(CRFB_ERROR, "Failed to start sdl");
		return 1;
	}

	pthread_t mouseThread, screenUpdateThread;
	CRFB_LOG(CRFB_INFO, "Started mouse thread");
    pthread_create(&mouseThread, NULL, *handle_mouse_input, (void *) &app);
	CRFB_LOG(CRFB_INFO, "Started screen update thread");
    pthread_create(&screenUpdateThread, NULL, *screen_update_thread, (void *) &app);

	while (!app.exitFlag) {
		handle_sdl_events(&app);

		int pitch;
		SDL_RenderClear(app.context->renderer);
		SDL_LockTexture(app.context->texture, &window_rect, &app.pixels, &pitch);
		
		memcpy(app.pixels, app.buffer->data, (app.buffer->width * app.buffer->height) * 4);

		SDL_UnlockTexture(app.context->texture);
		SDL_RenderTexture(app.context->renderer, app.context->texture, &window_rect_f, &window_rect_f);
		SDL_RenderPresent(app.context->renderer);
	}

	crfb_free_frame_buffer(app.buffer);

	crfb_client_close(app.client);
	crfb_free_client(app.client);

    SDL_DestroyTexture(app.context->texture);
    SDL_DestroyRenderer(app.context->renderer);
    SDL_DestroyWindow(app.context->window);

	free(app.context);

    return 0;
}