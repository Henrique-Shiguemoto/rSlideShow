#include "rSlide.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

static int allowed_font_names_windows_count = 164;
static const char* allowed_font_names_windows[] = {
	"ANTQUAB", "ANTQUABI", "ANTQUAI", "arial", "arialbd", "arialbi", "ariali", "ARIALN", "ARIALNB", "ARIALNBI", "ARIALNI", "ariblk", "bahnschrift", "BKANT", "BOOKOS", "BOOKOSB", "BOOKOSBI", "BOOKOSI", "BSSYM7", "calibri", "calibrib", "calibrii", "calibril", "calibrili", "calibriz", "cambriab", "cambriai", "cambriaz", "Candara", "Candarab", "Candarai", "Candaral", "Candarali", "Candaraz", "CascadiaCode", "CascadiaMono", "CENTURY", "comic", "comicbd", "comici", "comicz", "consola", "consolab", "consolai", "consolaz", "constan", "constanb", "constani", "constanz", "corbel", "corbelb", "corbeli", "corbell", "corbelli", "corbelz", "cour", "courbd", "courbi", "couri", "DUBAI-BOLD", "DUBAI-LIGHT", "DUBAI-MEDIUM", "DUBAI-REGULAR", "ebrima", "ebrimabd", "framd", "framdit", 
	"Gabriola", "gadugi", "gadugib", "GARA", "GARABD", "GARAIT", "georgia", "georgiab", "georgiai", "georgiaz", "GOTHIC", "GOTHICB", "GOTHICBI", "GOTHICI", "himalaya", "impact", "Inkfree", "javatext", "LeelaUIb", "LeelawUI", "LeelUIsl", "lucon", "l_10646", "malgun", "malgunbd", "malgunsl", "marlett", "micross", "mmrtext", "mmrtextb", "monbaiti", "msyi", "MTCORSVA", "MTEXTRA", "mvboli", "ntailu", "ntailub", "pala", "palab", "palabi", "palai", "phagspa", "phagspab", "REFSAN", "REFSPCL", "SansSerifCollection", "segmdl2", "SegoeIcons", "segoepr", "segoeprb", "segoesc", "segoescb", "segoeui", "segoeuib", "segoeuii", "segoeuil", "segoeuisl", "segoeuiz", "seguibl", "seguibli", "seguiemj", "seguihis", "seguili", "seguisb", "seguisbi", "seguisli", "seguisym", "SegUIVar",
	"simsunb", "SimsunExtG", "SitkaVF-Italic", "SitkaVF", "sylfaen", "symbol", "tahoma", "tahomabd", "taile", "taileb", "times", "timesbd", "timesbi", "timesi", "trebuc", "trebucbd", "trebucbi", "trebucit", "verdana", "verdanab", "verdanai", "verdanaz", "webdings", "wingding", "WINGDNG2", "WINGDNG3", "yumin", "yumindb", "yuminl"
};

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

	int line = 0;
	float x = 0.0f;
	float y = 0.0f;
	unsigned int color = 0;
	int font_size = 0;
	rs_string text = rs_create(NULL);
	rs_string text2 = rs_create(NULL);
	float width = 0.0f;
	float height = 0.0f;

	RLOGGER_INFO("Parsing %s", filepath);
	
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
				int convert_result = rs_convert_to_float(&token, &x);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("x parameter at line %d is invalid, setting to 0.0.", line);
					x = 0.0f;
				}
				if(x < 0.0f || 1.0f < x){
					RLOGGER_WARN("x parameter at line %d is outside of valid bounds, setting to 0.0.", line);
					x = 0.0f;
				}
			} else if (rs_starts_with_substring(&token, "y") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 4));
				int convert_result = rs_convert_to_float(&token, &y);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("y parameter at line %d is invalid, setting to 0.0.", line);
					y = 0.0f;
				}
				if(y < 0.0f || 1.0f < y){
					RLOGGER_WARN("y parameter at line %d is outside of valid bounds, setting to 0.0.", line);
					y = 0.0f;
				}
			} else if (rs_starts_with_substring(&token, "color") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 8));
				int color_result = rs_convert_hex_to_uint(&token, &color);
				if(color_result == RS_FAILURE){
					RLOGGER_WARN("color parameter at line %d is invalid, setting to black.", line);
					color = 0x00000000;
				}
			} else if (rs_starts_with_substring(&token, "text") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 7));
				R_ASSERT(rs_trim_delimiter(&token, '"'));
				R_ASSERT(rs_copy(&token, &text));
			} else if (rs_starts_with_substring(&token, "font_size") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 12));
				int convert_result = rs_convert_to_int(&token, &font_size);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("font_size parameter at line %d is invalid, setting to 20.", line);
					font_size = 20;
				}
				if(font_size < 0 || 160 < font_size){
					RLOGGER_WARN("font_size parameter at line %d is outside of bounds, setting to 20.", line);
					font_size = 20;
				}
			}else if (rs_starts_with_substring(&token, "font") != RS_FAILURE){
				R_ASSERT(rs_extract_right(&token, token.length - 7));
				R_ASSERT(rs_trim_delimiter(&token, '"'));

				//check if font is available
				int font_exists = 0;
				for (int i = 0; i < allowed_font_names_windows_count; ++i){
					if(rs_compare_to_cstr(&token, allowed_font_names_windows[i])){
						font_exists = 1;
						break;
					}
				}

				if(font_exists == 0){
					RLOGGER_WARN("font parameter at line %d isn't available, setting to arial.", line);
					R_ASSERT(rs_set(&text2, "arial"));
				}else{
					R_ASSERT(rs_copy(&token, &text2));
				}

				rText text_result = rtext_create(text.buffer, x, y, font_size, color, text2.buffer);
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
				int convert_result = rs_convert_to_float(&token, &x);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("x parameter at line %d is invalid, setting to 0.0.", line);
					x = 0.0f;
				}
				if(x < 0.0f || 1.0f < x){
					RLOGGER_WARN("x parameter at line %d is outside of valid bounds, setting to 0.0.", line);
					x = 0.0f;
				}
			} else if (rs_starts_with_substring(&token, "y") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 4));
				int convert_result = rs_convert_to_float(&token, &y);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("y parameter at line %d is invalid, setting to 0.0.", line);
					y = 0.0f;
				}
				if(y < 0.0f || 1.0f < y){
					RLOGGER_WARN("y parameter at line %d is outside of valid bounds, setting to 0.0.", line);
					y = 0.0f;
				}
			} else if (rs_starts_with_substring(&token, "width") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 8));
				int convert_result = rs_convert_to_float(&token, &width);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("width parameter at line %d is invalid, setting to 0.0.", line);
					width = 0.0f;
				}
				if(width < 0.0f){
					RLOGGER_WARN("width parameter at line %d is outside of bounds, setting to 0.0.", line);
					width = 0.0f;
				}
			} else if (rs_starts_with_substring(&token, "height") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 9));
				int convert_result = rs_convert_to_float(&token, &height);
				if(convert_result == RS_FAILURE){
					RLOGGER_WARN("height parameter at line %d is invalid, setting to 0.0.", line);
					height = 0.0f;
				}
				if(height < 0.0f){
					RLOGGER_WARN("height parameter at line %d is outside of bounds, setting to 0.0.", line);
					height = 0.0f;
				}
			} else if (rs_starts_with_substring(&token, "file_path") != RS_FAILURE) {
				R_ASSERT(rs_extract_right(&token, token.length - 12));
				R_ASSERT(rs_trim_delimiter(&token, '"'));
				R_ASSERT(rs_copy(&token, &text));
				rImage img_result = rimage_create(text.buffer, x, y, width, height);
				rdarray_push(&(slide.image_array), &img_result);

				// We don't actually need to free the rstring here, because the only place we change 
				// 		it is in the rs_copy function call, which already frees the memory before copying,
				// 		then after the outer most while loop, we call rs_delete on text, which frees it 
				// 		for the last time.
				x = 0.0;
				y = 0.0;
				width = 0.0;
				height = 0.0;

				// Just to make sure, I'm going to set both flags to 0
				next_token_is_a_txt_parameter = 0;
				next_token_is_a_img_parameter = 0;
				next_token_is_a_background_parameter = 0;
			}
		}else if(next_token_is_a_background_parameter){
			if(rs_starts_with_substring(&token, "color") != RS_FAILURE){
				R_ASSERT(rs_extract_right(&token, token.length - 8));
				int color_result = rs_convert_hex_to_uint(&token, &color);
				if(color_result == RS_FAILURE){
					RLOGGER_WARN("color parameter at line %d is invalid, setting to black.", line);
					color = 0x00000000;
				}
				slide.background_color = color;
			}
		}
		result_split = rs_split_by_delimiter(&file_contents, '\n', &token);
		line++;
	}

	rs_delete(&text);
	rs_delete(&text2);
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

rText rtext_create(const char* text, float x, float y, int font_size, unsigned int color, const char* font_name){
	R_ASSERT(text != NULL);
	R_ASSERT(font_name != NULL);
	R_ASSERT(global_state.window_width > 0);
   	R_ASSERT(global_state.window_height > 0);

   	rText text_result = {0};
	text_result.x = 2 * x - 1;
	text_result.y = -(2 * y - 1);
	text_result.font_size = font_size;
	text_result.color = color;

	texture_create(&text_result.texture_id);

	rs_string path = rs_create("C:\\Windows\\Fonts\\");
	rs_string font_name_string = rs_create(font_name);
	rs_string font_extension_string = rs_create(".ttf");
	R_ASSERT(rs_concatenate(&path, &font_name_string));
	R_ASSERT(rs_concatenate(&path, &font_extension_string));
	TTF_Font* font = TTF_OpenFont(path.buffer, text_result.font_size);
	if(font == NULL){
		RLOGGER_ERROR("TTF_OpenFont(): %s", TTF_GetError());
		return text_result;
	}
	rs_delete(&font_extension_string);
	rs_delete(&font_name_string);
	rs_delete(&path);

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, (SDL_Color){COLOR_HEX_TO_UINT8s(text_result.color)});
	if (text_surface) {
		SDL_Surface* alpha_image = SDL_CreateRGBSurface(SDL_SWSURFACE, text_surface->w, text_surface->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_BlitSurface(text_surface, NULL, alpha_image, NULL);
		SDL_FlipSurfaceVertical(alpha_image);

		texture_set_data(&text_result.texture_id, alpha_image->pixels, alpha_image->w, alpha_image->h, 1);
		SDL_FreeSurface(alpha_image);
		
		RLOGGER_INFO("Succesfully created SDL_Surface for: %s", text);
   	} else {
   		RLOGGER_WARN("Failed to create SDL Surface for: %s", text);
   		return text_result;
   	}
   	TTF_CloseFont(font);

   	float width_norm  = (float)text_surface->w / global_state.window_width;
   	float height_norm = (float)text_surface->h / global_state.window_height;
	float vertex[] = {
		//x    	 							y    								z     u    	v
		text_result.x, 	 					text_result.y, 						0.0f, 0.0f, 1.0f, // top - left
		text_result.x + width_norm,  		text_result.y, 						0.0f, 1.0f, 1.0f, // top - right
		text_result.x,    					text_result.y - height_norm, 		0.0f, 0.0f, 0.0f, // bot - left
		text_result.x,    					text_result.y - height_norm, 		0.0f, 0.0f, 0.0f, // bot - left
		text_result.x + width_norm,  		text_result.y, 						0.0f, 1.0f, 1.0f, // top - right
		text_result.x + width_norm,  		text_result.y - height_norm, 		0.0f, 1.0f, 0.0f  // bot - right
	};

	RLOGGER_INFO("Vertices for: %s", text);
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
		text->x = 0;
		text->y = 0;
		text->font_size = 0;
		text->color = 0;
		vao_delete(&text->vao_id);
		text->vao_id = 0xB01DFACE;
		vao_delete(&text->vbo_id);
		text->vbo_id = 0xB01DFACE;
		texture_delete(&text->texture_id);
		text->texture_id = 0xB01DFACE;
	}
}

rImage rimage_create(const char* filepath, float x, float y, float width, float height){
	R_ASSERT(filepath != NULL);
	R_ASSERT(global_state.window_width > 0);
   	R_ASSERT(global_state.window_height > 0);

	rImage img_result = {0};
	// converted to be normalized [-1, 1]
	img_result.x = 2 * x - 1;
	img_result.y = -(2 * y - 1);

	// converted to be normalized [0, 2]
	img_result.width  = 2.0f * (width / global_state.window_width);
	img_result.height = 2.0f * (height / global_state.window_height);

	texture_create(&img_result.texture_id);

	stbi_set_flip_vertically_on_load(1);
	int width_file, height_file, nChannels;
   	unsigned char *data = stbi_load(filepath, &width_file, &height_file, &nChannels, 0);
   	if (data) {
   		texture_set_data(&img_result.texture_id, data, width_file, height_file, 1);
		RLOGGER_INFO("Succesfully loaded image: %s", filepath);
   	} else {
   		RLOGGER_WARN("Failed to load texture: %s", filepath);
   		return img_result;
   	}
   	stbi_image_free(data);

	float vertex[] = {
		//x    	 							y    								z     u    	v
		img_result.x, 	 					img_result.y, 						0.0f, 0.0f, 1.0f, // top - left
		img_result.x + img_result.width,   	img_result.y, 						0.0f, 1.0f, 1.0f, // top - right
		img_result.x,    					img_result.y - img_result.height, 	0.0f, 0.0f, 0.0f, // bot - left
		img_result.x,    					img_result.y - img_result.height, 	0.0f, 0.0f, 0.0f, // bot - left
		img_result.x + img_result.width,   	img_result.y, 						0.0f, 1.0f, 1.0f, // top - right
		img_result.x + img_result.width,  	img_result.y - img_result.height, 	0.0f, 1.0f, 0.0f  // bot - right
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
		image->x = 0;
		image->y = 0;
		image->width = 0;
		image->height = 0;
		vao_delete(&image->vao_id);
		image->vao_id = 0xB01DFACE;
		vao_delete(&image->vbo_id);
		image->vbo_id = 0xB01DFACE;
		texture_delete(&image->texture_id);
		image->texture_id = 0xB01DFACE;
	}
}

// From SDL_surface.c from SDL3
int SDL_FlipSurfaceVertical(SDL_Surface *surface)
{
    Uint8 *a, *b, *tmp;
    int i;

    if (surface->h <= 1) {
        return 1;
    }

    a = (Uint8 *)surface->pixels;
    b = a + (surface->h - 1) * surface->pitch;
    tmp = SDL_malloc(sizeof(unsigned int) * surface->pitch);
    if (!tmp) {
        return 0;
    }
    for (i = surface->h / 2; i--; ) {
        SDL_memcpy(tmp, a, surface->pitch);
        SDL_memcpy(a, b, surface->pitch);
        SDL_memcpy(b, tmp, surface->pitch);
        a += surface->pitch;
        b -= surface->pitch;
    }
    SDL_free(tmp);
    return 1;
}
