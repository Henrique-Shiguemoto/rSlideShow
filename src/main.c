#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL.h"

int main(void){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Error SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	printf("Initialized SDL2\n");
	SDL_Quit();
	return 0;
}
