#include "rTexture.h"

void texture_create(unsigned int* texture_id){
	glGenTextures(1, texture_id);
	texture_bind(texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void texture_set_data(unsigned int* texture_id, void* pixel_data, unsigned int width, unsigned int height, int generate_mipmap){
	texture_bind(texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
	if(generate_mipmap != 0){
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	texture_unbind();
}

void texture_bind(unsigned int* texture_id){
	glBindTexture(GL_TEXTURE_2D, *texture_id);
}

void texture_unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(unsigned int* texture_id){
	glDeleteTextures(1, texture_id);
}
