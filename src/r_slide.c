#include "r_slide.h"

rSlide rslide_create(const char* filepath){
	rio_file* file = rio_open_file(filepath, RIO_READ_MODE);
	if(!file){
		RLOGGER_ERROR("%s%s", "Couldn't find this file: ", filepath);
		return (rSlide){0};
	}
	
	rs_string file_contents = rs_create((const char*)file->contents);
	if(!file_contents.buffer){
		RLOGGER_ERROR("%s", "Couldn't extract file contents from input file");
		return (rSlide){0};
	}

	rSlide slide = {0};
	slide.image_array = rdarray_create(sizeof(rImage));
	slide.text_array = rdarray_create(sizeof(rText));
	
	// TODO(Rick): parse file contents
	// get all [TXT]s contents
	// get all [IMG]s contents

	rs_delete(&file_contents);
	rio_close_file(file);
	return slide;
}

void rslide_delete(rSlide* slide){
	if(slide){
		rdarray_delete(&(slide->image_array));
		rdarray_delete(&(slide->text_array));
		slide->background_color = 0;
	}
}

// TODO(Rick): Implement these functions below

rText rtext_create(const char* text, int x, int y, int font_size, unsigned int color){
	return (rText){0};
}

void rtext_delete(rText* text){

}

rImage rimage_create(const char* filepath, int x, int y, int width, int height){
	return (rImage){0};
}

void rimage_delete(rImage* image){
	
}
