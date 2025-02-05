#ifndef R_SLIDE_H
#define R_SLIDE_H

#include <glad/glad.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "rInout.h"
#include "rStrlib.h"
#include "rDarray.h"
#include "rLogger.h"
#include "rTexture.h"
#include "rVBO.h"
#include "rVAO.h"
#include "rGlobal.h"

#define COLOR_HEX_TO_UINT8s(_color) (((_color) >> 24) & 0x000000FF), \
									(((_color) >> 16) & 0x000000FF), \
									(((_color) >> 8)  & 0x000000FF), \
									(((_color) >> 0)  & 0x000000FF)

typedef enum rEntityTag {
	R_ENTITY_TEXT,
	R_ENTITY_IMAGE,
	R_ENTITY_COUNT
} rEntityTag;

typedef struct rImage {
	float x;
	float y;
	float width;
	float height;
	int layer;
	unsigned int vao_id;
	unsigned int vbo_id;
	unsigned int texture_id;
} rImage;

typedef struct rText {
	float x;
	float y;
	int font_size;
	int layer;
	unsigned int color;
	unsigned int vao_id;
	unsigned int vbo_id;
	unsigned int texture_id;
} rText;

typedef struct rEntity {
    rEntityTag tag;
    union {
        rText text;
		rImage image;
    } data;
} rEntity;

typedef struct rSlide {
	rdarray entity_array;
	unsigned int background_color;
} rSlide;

rSlide rslide_create(const char* filepath);
void rslide_delete(rSlide* slide);

rText rtext_create(const char* text, float x, float y, int font_size, unsigned int color, const char* font, int layer);
void rtext_delete(rText* text);

rImage rimage_create(const char* filepath, float x, float y, float width, float height, int layer);
void rimage_delete(rImage* image);

int SDL_FlipSurfaceVertical(SDL_Surface* surface);

#endif // R_SLIDE_H 