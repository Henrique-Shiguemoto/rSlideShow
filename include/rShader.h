#ifndef R_SHADER_H
#define R_SHADER_H

#include "rMathlib.h"
#include "glad/glad.h"
#include "rLogger.h"
#include "rInout.h"

int create_shaders(const char* vertex_shader_path, const char* fragment_shader_path);
void shader_use(unsigned int shader_id);
void shader_delete(unsigned int shader_id);
void shader_set_bool(unsigned int shader_id, const char* uniform_name, int value);
void shader_set_int(unsigned int shader_id, const char* uniform_name, int value);
void shader_set_float(unsigned int shader_id, const char* uniform_name, float value);
void shader_set_vec2(unsigned int shader_id, const char* uniform_name, rm_v2f value);
void shader_set_vec3(unsigned int shader_id, const char* uniform_name, rm_v3f value);
void shader_set_vec4(unsigned int shader_id, const char* uniform_name, rm_v4f value);
void shader_set_mat2(unsigned int shader_id, const char* uniform_name, rm_mat2f value);
void shader_set_mat3(unsigned int shader_id, const char* uniform_name, rm_mat3f value);
void shader_set_mat4(unsigned int shader_id, const char* uniform_name, rm_mat4f value);

#endif // R_SHADER_H