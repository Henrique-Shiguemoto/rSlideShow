#include "r_slide.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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
	slide.text_array = rdarray_create(sizeof(rText));
	slide.image_array = rdarray_create(sizeof(rImage));
	
	// TODO(Rick): Debug parsing file contents
	rs_string token = rs_create(NULL);
	int result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
	int next_token_is_a_txt_parameter = 0;
	int next_token_is_a_img_parameter = 0;

	float x = 0.0;
	float y = 0.0;
	unsigned int color = 0;
	int font_size = 0;
	rs_string text = rs_create(NULL); // used for file_path in [IMG] and text in [TXT]
	int width = 0;
	int height = 0;

	while(result_split != RS_FAILURE){
		rs_trim(&token);
		if(rs_starts_with_substring(&token, "[TXT]")){
			next_token_is_a_txt_parameter = 1;
			next_token_is_a_img_parameter = 0;
			continue;
		}else if(rs_starts_with_substring(&token, "[IMG]")){
			next_token_is_a_txt_parameter = 0;
			next_token_is_a_img_parameter = 1;
			continue;
		}

		// TODO(Rick): Add error checking to all r_strlib function calls
		if(next_token_is_a_txt_parameter){
			if (rs_starts_with_substring(&token, "x") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 4);
				rs_convert_to_float(&token, &x);
			} else if (rs_starts_with_substring(&token, "y") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 4);
				rs_convert_to_float(&token, &y);
			} else if (rs_starts_with_substring(&token, "color") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 8);
				// TODO(Rick): Convert color from string to unsigned int without using strtoul()
				char* _temp = NULL;
				color = strtoul(token.buffer, &_temp, 16);
			} else if (rs_starts_with_substring(&token, "text") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 7);
				rs_copy(&token, &text);
			} else if (rs_starts_with_substring(&token, "font_size") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 12);
				rs_convert_to_int(&token, &font_size);
				rText text_result = rtext_create(text.buffer, x, y, font_size, color);
				rdarray_push(&(slide.text_array), &text_result);
				
				// We don't actually need to free the rstring here, because the only place we change 
				// 		it is in the rs_copy function call, which already frees the memory before copying,
				// 		then after the outer most while loop, we call rs_delete on text, which frees it 
				// 		for the last time.
				x = 0.0;
				y = 0.0;
				font_size = 0;
				color = 0;

				// Just to make sure, I'm going to set both flags to 0
				next_token_is_a_txt_parameter = 0;
				next_token_is_a_img_parameter = 0;
			}
		}else if(next_token_is_a_img_parameter){
			if (rs_starts_with_substring(&token, "x") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 4);
				rs_convert_to_float(&token, &x);
			} else if (rs_starts_with_substring(&token, "y") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 4);
				rs_convert_to_float(&token, &y);
			} else if (rs_starts_with_substring(&token, "width") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 8);
				rs_convert_to_int(&token, &width);
			} else if (rs_starts_with_substring(&token, "height") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 9);
				rs_convert_to_int(&token, &height);
			} else if (rs_starts_with_substring(&token, "file_path") != RS_FAILURE) {
				rs_extract_right(&token, token.length - 12);
				rs_copy(&token, &text);
				rImage text_result = rimage_create(text.buffer, x, y, font_size, color);
				rdarray_push(&(slide.image_array), &text_result);

				// We don't actually need to free the rstring here, because the only place we change 
				// 		it is in the rs_copy function call, which already frees the memory before copying,
				// 		then after the outer most while loop, we call rs_delete on text, which frees it 
				// 		for the last time.
				x = 0.0;
				y = 0.0;
				width = 0;
				height = 0;

				// Just to make sure, I'm going to set both flags to 0
				next_token_is_a_txt_parameter = 0;
				next_token_is_a_img_parameter = 0;
			}
		}
		result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
	}

	rs_delete(&text);
	rs_delete(&token);
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

rText rtext_create(const char* text, float x, float y, int font_size, unsigned int color){
	return (rText){.text = text, .x = x, .y = y, .font_size = font_size, .color = color};
}

void rtext_delete(rText* text){
	if(!text){
		text->text = 0;
		text->x = 0;
		text->y = 0;
		text->font_size = 0;
		text->color = 0;
	}
}

rImage rimage_create(const char* filepath, float x, float y, int width, int height){
	int x1,y1,n1;
   	unsigned char *data = stbi_load(filepath, &x1, &y1, &n1, 0);
	return (rImage){.pixel_data = data, .x = x, .y = y, .width = width, .height = height};
}

void rimage_delete(rImage* image){
	if(!image){
		stbi_image_free(image->pixel_data);
		image->pixel_data = 0;
		image->x = 0;
		image->y = 0;
		image->width = 0;
		image->height = 0;
	}
}

