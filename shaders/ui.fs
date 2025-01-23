#version 330 core

out vec4 outputColor;
in vec2 textureCoordFromVertexShader;

uniform sampler2D tex;

void main(){
	outputColor = texture(tex, textureCoordFromVertexShader);
}