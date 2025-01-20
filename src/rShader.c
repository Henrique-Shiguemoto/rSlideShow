#include "rShader.h"

int create_shaders(const char* vertex_shader_path, const char* fragment_shader_path){
	R_ASSERT(vertex_shader_path != NULL && fragment_shader_path != NULL);
	
	rio_file* vertex_shader_file = rio_open_file(vertex_shader_path, RIO_READ_MODE);
	rio_file* fragment_shader_file = rio_open_file(fragment_shader_path, RIO_READ_MODE);

	unsigned int vertexShaderID, fragmentShaderID;
	int success;
	char infoLog[512];

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, (char const * const *)&(vertex_shader_file->contents), NULL);
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		RLOGGER_WARN("Failed to compile vertex shader: %s", infoLog);
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, (char const * const *)&(fragment_shader_file->contents), NULL);
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		RLOGGER_WARN("Failed to compile fragment shader: %s", infoLog);
	}

	unsigned int shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertexShaderID);
	glAttachShader(shader_program_id, fragmentShaderID);
	glLinkProgram(shader_program_id);
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shader_program_id, 512, NULL, infoLog);
		RLOGGER_WARN("Failed to link shader program: %s", infoLog);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	return shader_program_id;
}

void shader_use(unsigned int shader_id) 												{ glUseProgram(shader_id); }
void shader_delete(unsigned int shader_id) 												{ glDeleteProgram(shader_id); }
void shader_set_bool(unsigned int shader_id, const char* uniform_name, int value) 		{ glUniform1i(glGetUniformLocation(shader_id, uniform_name), value); }
void shader_set_int(unsigned int shader_id, const char* uniform_name, int value) 		{ glUniform1i(glGetUniformLocation(shader_id, uniform_name), value); }
void shader_set_float(unsigned int shader_id, const char* uniform_name, float value) 	{ glUniform1f(glGetUniformLocation(shader_id, uniform_name), value); }
void shader_set_vec2(unsigned int shader_id, const char* uniform_name, rm_v2f value) 	{ glUniform2fv(glGetUniformLocation(shader_id, uniform_name), 1, (GLfloat*)&value); }
void shader_set_vec3(unsigned int shader_id, const char* uniform_name, rm_v3f value) 	{ glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, (GLfloat*)&value); }
void shader_set_vec4(unsigned int shader_id, const char* uniform_name, rm_v4f value) 	{ glUniform4fv(glGetUniformLocation(shader_id, uniform_name), 1, (GLfloat*)&value); }
void shader_set_mat2(unsigned int shader_id, const char* uniform_name, rm_mat2f value) 	{ glUniformMatrix2fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &(value.elem[0])); }
void shader_set_mat3(unsigned int shader_id, const char* uniform_name, rm_mat3f value) 	{ glUniformMatrix3fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &(value.elem[0])); }
void shader_set_mat4(unsigned int shader_id, const char* uniform_name, rm_mat4f value) 	{ glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &(value.elem[0])); }