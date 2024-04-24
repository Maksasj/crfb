#include <SDL3/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 270

int main(){
    SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

	window = SDL_CreateWindow("CRFB Client [192.168.1.116:5900]", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if(window == NULL) {
		printf("Failed to create SDL window %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_SOFTWARE);
	if(renderer == NULL) {
		printf("Failed to create SDL renderer %s\n", SDL_GetError());
	}

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

	const SDL_FRect window_rect_f = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};
	const SDL_Rect window_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	void *pixels;

    int pitch;
    SDL_LockTexture(texture, &window_rect, &pixels, &pitch);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // choose color to draw, rgba
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
    SDL_RenderPoint(renderer, WINDOW_WIDTH/2, WINDOW_HEIGHT/2); // draw pixel in center

	unsigned char exit = 0;
	while (!exit) {
		SDL_UnlockTexture(texture);
		SDL_RenderTexture(renderer, texture, &window_rect_f, &window_rect_f);

		SDL_RenderPresent(renderer);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT: {
					exit = 1;
					break;
				}
			}

			for(int x = 0; x < WINDOW_WIDTH; ++x) {
				for(int y = 0; y < WINDOW_HEIGHT; ++y) {
					((unsigned int*) pixels)[x + y*WINDOW_WIDTH] = 0xff00ffff;
				}
			}
		}

		SDL_RenderClear(renderer);
		int pitch;
		SDL_LockTexture(texture, &window_rect, &pixels, &pitch);
	}

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}