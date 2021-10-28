//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include "SDL_helpers.h" //my own code
#include <stdlib.h>
#include <pthread.h>

#include "bit_manipulation.h"
#include "MIPS_parser.h"
#include "MIPS_memory.h"
#include "text_manager.h"

#define SCREEN_WIDTH 512	//window height
#define SCREEN_HEIGHT 256	//window width

//function prototypes
//initilise SDL
int init(int w, int h, int argc, char *args[]);

// Program globals
int width, height;		//used if fullscreen

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen

//surfaces
static SDL_Surface *screen;

//textures
SDL_Texture *screen_texture;

// quit global var
int quit = 0;
pthread_mutex_t stopMutex;

int getQuit() {
  int ret = 0;
  pthread_mutex_lock(&stopMutex);
  ret = quit;
  pthread_mutex_unlock(&stopMutex);
  return ret;
}

void setQuit(int val) {
  pthread_mutex_lock(&stopMutex);
  quit = val;
  pthread_mutex_unlock(&stopMutex);
}

void *renderLoop();
int updateDisplay(SDL_Surface *screen);

int main (int argc, char *args[]) {
		
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {

		return 0;
	}

	SDL_GetWindowSize(window, &width, &height);

	// MIPS setup
	init_memory();
	openText();

	//render loop
	pthread_t thread_id;
	pthread_create( &thread_id, NULL, renderLoop, NULL);

	//instruction loop
	while(getQuit() == 0){
		int ret = readNextInstruction();
		if(ret)
			setQuit(1);
	}
	pthread_join( thread_id, NULL);
	

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);

	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	// Close the instructions file and free all the emulated memory
	closeText();
	free_memory();
	return 0;
	
}

void *renderLoop(){
	int sleep = 0;
	Uint32 next_game_tick = SDL_GetTicks();

	while(getQuit() == 0) {

		//check for new events every frame
		SDL_PumpEvents();

		// Update the display
		updateDisplay(screen);

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_ESCAPE]) 
			setQuit(1);

		else{
			;	
			// Aquí ocupo escribir las teclas que se escriben en memoria en el lugar correcto
			//// player movement
			//if (keystate[SDL_SCANCODE_DOWN]) 
			//	move_player(DOWN, state);
			//else if (keystate[SDL_SCANCODE_UP]) 
			//	move_player(UP, state);
			//else if (keystate[SDL_SCANCODE_RIGHT]) 
			//	move_player(RIGHT, state);
			//else if (keystate[SDL_SCANCODE_LEFT]) 
			//	move_player(LEFT, state);
			//else
			//	move_player(NOWHERE, state);
		}

		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof (Uint32));
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

		//draw to the display
		SDL_RenderPresent(renderer);

		//time it takes to render  frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();

		if( sleep >= 0 ) {

			SDL_Delay(sleep);
		}
	}
}

int updateDisplay(SDL_Surface *screen) {

	for(int i = 0; i<(SCREEN_WIDTH*SCREEN_HEIGHT/(8*8)); i++){ // pixels are 8x8
		if(getFromHeap(i) == 0)
			continue;
		printf("AAAAA");
		int x = 8*(i%64);
		int y = 8*(i/64);
		SDL_Rect src = instantiateRect(x, y, 8, 8);
		int r = SDL_FillRect(screen , &src, SDL_MapRGB(screen->format, 28, 87, 254));

		if (r !=0){
			printf("Fill rectangle failed during updateDisaply() iteration #%d\n", i);
		}
	}
	return 0;
}

int init(int width, int height, int argc, char *args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		//
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
		}
	}

	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	//create the screen sruface where all the elemnts will be drawn onto 
	screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
	
	if (screen == NULL) {
		
		printf("Could not create the screen surfce! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//create the screen texture to render the screen surface to the actual display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (screen_texture == NULL) {
		
		printf("Could not create the screen_texture! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	return 0;
}

