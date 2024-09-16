#version 450 core

uniform sampler2D color_map;

in vec2 fragUV;
out vec4 col;

void main(){
	col = texture(color_map, fragUV);
}
