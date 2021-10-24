#include "SDL_helpers.h"

SDL_Rect instantiateRect(int x, int y, int w, int h){
	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	rect.w=w;
	rect.h=h;
	return rect;
}

