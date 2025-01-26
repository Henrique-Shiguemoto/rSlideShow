#ifndef R_TEXTURE_H
#define R_TEXTURE_H

#include <glad/glad.h>

void texture_create(unsigned int* texture_id);
void texture_set_data(unsigned int* texture_id, void* pixel_data, unsigned int width, unsigned int height, int generate_mipmaps);
void texture_bind(unsigned int* texture_id);
void texture_unbind();
void texture_delete(unsigned int* texture_id);

#endif // R_TEXTURE_H