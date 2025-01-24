#include "rSlide.h"

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
	
	R_ASSERT(rs_remove_delimiter(&file_contents, '\r'));
	R_ASSERT(rs_remove_delimiter(&file_contents, '\t'));

	rs_string token = rs_create(NULL);
	int result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
	int next_token_is_a_txt_parameter = 0;
	int next_token_is_a_img_parameter = 0;
	int next_token_is_a_background_parameter = 0;

	float x = 0.0;
	float y = 0.0;
	unsigned int color = 0;
	int font_size = 0;
	rs_string text = rs_create(NULL); // used for file_path in [IMG] and text in [TXT]
	int width = 0;
	int height = 0;

	// TODO(Rick): check for .rslide parameters valid values, log them as warnings and set invalid values to default valid values
	while(result_split != RS_FAILURE){
		rs_trim(&token);
		if(rs_starts_with_substring(&token, "[TXT]")){
			next_token_is_a_txt_parameter = 1;
			next_token_is_a_img_parameter = 0;
			next_token_is_a_background_parameter = 0;
			result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
			continue;
		}else if(rs_starts_with_substring(&token, "[IMG]")){
			next_token_is_a_txt_parameter = 0;
			next_token_is_a_img_parameter = 1;
			next_token_is_a_background_parameter = 0;
			result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
			continue;
		}else if(rs_starts_with_substring(&token, "[BACKGROUND]")){
			next_token_is_a_txt_parameter = 0;
			next_token_is_a_img_parameter = 0;
			next_token_is_a_background_parameter = 1;
			result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
			continue;
		}

		if(next_token_is_a_txt_parameter){
			if (rs_starts_with_substring(&token, "x") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 4));
				R_ASSERT(rs_convert_to_float(&token, &x));
			} else if (rs_starts_with_substring(&token, "y") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 4));
				R_ASSERT(rs_convert_to_float(&token, &y));
			} else if (rs_starts_with_substring(&token, "color") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 8));
				R_ASSERT(rs_convert_hex_to_uint(&token, &color));
			} else if (rs_starts_with_substring(&token, "text") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 7));
				R_ASSERT(rs_trim_delimiter(&token, '"'));
				R_ASSERT(rs_copy(&token, &text));
			} else if (rs_starts_with_substring(&token, "font_size") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 12));
				R_ASSERT(rs_convert_to_int(&token, &font_size));
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
				next_token_is_a_background_parameter = 0;
			}
		}else if(next_token_is_a_img_parameter){
			if (rs_starts_with_substring(&token, "x") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 4));
				R_ASSERT(rs_convert_to_float(&token, &x));
			} else if (rs_starts_with_substring(&token, "y") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 4));
				R_ASSERT(rs_convert_to_float(&token, &y));
			} else if (rs_starts_with_substring(&token, "width") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 8));
				R_ASSERT(rs_convert_to_int(&token, &width));
			} else if (rs_starts_with_substring(&token, "height") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 9));
				R_ASSERT(rs_convert_to_int(&token, &height));
			} else if (rs_starts_with_substring(&token, "file_path") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 12));
				R_ASSERT(rs_trim_delimiter(&token, '"'));
				R_ASSERT(rs_copy(&token, &text));
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
				next_token_is_a_background_parameter = 0;
			}
		}else if(next_token_is_a_background_parameter){
			if(rs_starts_with_substring(&token, "color") != RS_FAILURE){
				R_ASSERT(rs_extract_right(&token, token.length - 8));
				R_ASSERT(rs_convert_hex_to_uint(&token, &color));
				slide.background_color = color;
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
		for(int i = 0; i < slide->image_array.length; i++){
			rimage_delete((rImage*)rdarray_at(&slide->image_array, i));
		}
		for(int i = 0; i < slide->text_array.length; i++){
			rtext_delete((rText*)rdarray_at(&slide->text_array, i));
		}

		rdarray_delete(&(slide->image_array));
		rdarray_delete(&(slide->text_array));
		slide->background_color = 0;
	}
}

rText rtext_create(const char* text, float x, float y, int font_size, unsigned int color){
	R_ASSERT(text != NULL);
	R_ASSERT(global_state.window_width > 0);
   	R_ASSERT(global_state.window_height > 0);
   	R_ASSERT(global_state.font != NULL);

	rText text_result = {0};
	text_result.text = text;
	text_result.x = 2 * x - 1;
	text_result.y = 2 * y - 1;
	text_result.font_size = font_size;
	text_result.color = color;

	glGenTextures(1, &text_result.texture_id);
	glBindTexture(GL_TEXTURE_2D, text_result.texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_Surface* text_surface = TTF_RenderText_Solid(global_state.font, text_result.text, (SDL_Color){COLOR_HEX_TO_UINT8s(text_result.color)});
	if (text_surface && text_surface->pixels) {
		text_result.pixel_data = text_surface->pixels;
   		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, text_surface->w, text_surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, text_surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		RLOGGER_INFO("Succesfully created SDL_Surface for: %s", text_result.text);
   	} else {
   		RLOGGER_WARN("Failed to create SDL Surface for: %s", text_result.text);
   		return text_result;
   	}

	float vertex[] = {
		//x    	 y    	z     u    	v
		-0.5f, 	 0.5f, 	0.0f, 0.0f, 1.0f, 
		 0.5f,   0.5f, 	0.0f, 1.0f, 1.0f, 
		-0.5f,  -0.5f, 	0.0f, 0.0f, 0.0f, 
		-0.5f,  -0.5f, 	0.0f, 0.0f, 0.0f, 
		 0.5f,   0.5f, 	0.0f, 1.0f, 1.0f, 
		 0.5f,  -0.5f, 	0.0f, 1.0f, 0.0f
	};

	RLOGGER_INFO("Vertices for: %s", text_result.text);
	RLOGGER_INFO("%s", "vertex[] = {");
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[0],   vertex[1],   vertex[2],   vertex[3],  vertex[4]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[5],   vertex[6],   vertex[7],   vertex[8],  vertex[9]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[10],  vertex[11],  vertex[12],  vertex[13], vertex[14]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[15],  vertex[16],  vertex[17],  vertex[18], vertex[19]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[20],  vertex[21],  vertex[22],  vertex[23], vertex[24]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f",  vertex[25],  vertex[26],  vertex[27],  vertex[28], vertex[29]);
	RLOGGER_INFO("%s", "}");

	vao_create(&text_result.vao_id);
	vao_bind(&text_result.vao_id);
	vbo_create(&text_result.vbo_id, vertex, sizeof(vertex));
	vao_define_vbo_layout(&text_result.vbo_id, 0, 3, 5 * sizeof(float), 0); // positions
	vao_define_vbo_layout(&text_result.vbo_id, 1, 2, 5 * sizeof(float), 3); // uvs

	return text_result;
}

void rtext_delete(rText* text){
	if(!text){
		// This assumes that SDL_Surface field order before the pixel data pointer is Uint32 flags; SDL_PixelFormat *format; int w, h; int pitch;
		SDL_FreeSurface((void*)((char*)text->pixel_data - 3 * sizeof(int) - sizeof(SDL_PixelFormat*) - sizeof(Uint32)));
		text->text = 0;
		text->x = 0;
		text->y = 0;
		text->font_size = 0;
		text->color = 0;
		text->vao_id = 0xB01DFACE;
		text->vbo_id = 0xB01DFACE;
		text->texture_id = 0xB01DFACE;
	}
}

rImage rimage_create(const char* filepath, float x, float y, int width, int height){
	R_ASSERT(filepath != NULL);

	rImage img_result = {0};
	img_result.x = 2 * x - 1;
	img_result.y = 2 * y - 1;
	img_result.width = width;
	img_result.height = height;

	glGenTextures(1, &img_result.texture_id);
	glBindTexture(GL_TEXTURE_2D, img_result.texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(1);
	int width_file, height_file, nChannels;
   	unsigned char *data = stbi_load(filepath, &width_file, &height_file, &nChannels, 0);
   	if (data) {
   		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_file, height_file, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		img_result.pixel_data = data;
		RLOGGER_INFO("Succesfully loaded image: %s", filepath);
   	} else {
   		RLOGGER_WARN("Failed to load texture: %s", filepath);
   		return img_result;
   	}

   	R_ASSERT(global_state.window_width > 0);
   	R_ASSERT(global_state.window_height > 0);
	float vertex[] = {
		//x    	 y    	z     u    	v
		-1.0f, 	 1.0f, 	0.0f, 0.0f, 1.0f, // top - left
		 1.0f,   1.0f, 	0.0f, 1.0f, 1.0f, // top - right
		-1.0f,  -1.0f, 	0.0f, 0.0f, 0.0f, // bot - left
		-1.0f,  -1.0f, 	0.0f, 0.0f, 0.0f, // bot - left
		 1.0f,   1.0f, 	0.0f, 1.0f, 1.0f, // top - right
		 1.0f,  -1.0f, 	0.0f, 1.0f, 0.0f  // bot - right
	};

	RLOGGER_INFO("Vertices for: %s", filepath);
	RLOGGER_INFO("%s", "vertex[] = {");
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[0],   vertex[1],   vertex[2],   vertex[3],  vertex[4]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[5],   vertex[6],   vertex[7],   vertex[8],  vertex[9]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[10],  vertex[11],  vertex[12],  vertex[13], vertex[14]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[15],  vertex[16],  vertex[17],  vertex[18], vertex[19]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f,", vertex[20],  vertex[21],  vertex[22],  vertex[23], vertex[24]);
	RLOGGER_INFO("\t%f, %f, %f, %f, %f",  vertex[25],  vertex[26],  vertex[27],  vertex[28], vertex[29]);
	RLOGGER_INFO("%s", "}");
	
	vao_create(&img_result.vao_id);
	vao_bind(&img_result.vao_id);
	vbo_create(&img_result.vbo_id, vertex, sizeof(vertex));
	vao_define_vbo_layout(&img_result.vbo_id, 0, 3, 5 * sizeof(float), 0); // positions
	vao_define_vbo_layout(&img_result.vbo_id, 1, 2, 5 * sizeof(float), 3); // uvs

	return img_result;
}

void rimage_delete(rImage* image){
	if(!image){
		stbi_image_free(image->pixel_data);
		image->pixel_data = 0;
		image->x = 0;
		image->y = 0;
		image->width = 0;
		image->height = 0;
		image->vao_id = 0xB01DFACE;
		image->vbo_id = 0xB01DFACE;
		image->texture_id = 0xB01DFACE;
	}
}

