#ifndef MAIN_H
#define MAIN_H

#include "r_slide.h"
#include "r_logger.h"
#include "r_darray.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "glad/glad.h"

int init_everything(SDL_Window** window, int width, int height, SDL_GLContext* gl_context);
void handle_input();
void render_graphics(SDL_Window** window);
void quit_everything(SDL_Window** window, SDL_GLContext* gl_context);

#endif // MAIN_H