#include "r_slide.h"

rSlide* rslide_create(const char* filepath){
	return 0;
}

void rslide_delete(rSlide* slide){
	// go through images and free them
	// go through texts and free them
	// set background_color to 0
}

rText* rtext_create(const char* text, int x, int y, int font_size, unsigned int color){
	return 0;
}

void rtext_delete(rText* text){

}

rImage* rimage_create(const char* filepath, int x, int y, int width, int height){
	return 0;
}

void rimage_delete(rImage* image){
	
}
