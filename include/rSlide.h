#ifndef R_SLIDE_H
#define R_SLIDE_H

#include <glad/glad.h>

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

typedef struct rImage {
	void* pixel_data;
	float x;
	float y;
	float width;
	float height;
	unsigned int vao_id;
	unsigned int vbo_id;
	unsigned int texture_id;
} rImage;

typedef struct rText {
	const char* text;
	void* pixel_data;
	float x;
	float y;
	int font_size;
	unsigned int color;
	unsigned int vao_id;
	unsigned int vbo_id;
	unsigned int texture_id;
} rText;

typedef struct rSlide {
	rdarray image_array;
	rdarray text_array;
	unsigned int background_color;
} rSlide;

rSlide rslide_create(const char* filepath);
void rslide_delete(rSlide* slide);

rText rtext_create(const char* text, float x, float y, int font_size, unsigned int color);
void rtext_delete(rText* text);

rImage rimage_create(const char* filepath, float x, float y, float width, float height);
void rimage_delete(rImage* image);

int SDL_FlipSurfaceVertical(SDL_Surface *surface);

#endif // R_SLIDE_H 