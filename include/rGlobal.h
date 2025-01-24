#ifndef R_GLOBAL_H
#define R_GLOBAL_H

#include "SDL2/SDL_ttf.h"

typedef struct rGlobal {
	const char* app_name;
	float window_width;
	float window_height;
	unsigned int slide_count;
	TTF_Font* font; // for now only one global font
} rGlobal;

extern rGlobal global_state;

#endif // R_GLOBAL_H