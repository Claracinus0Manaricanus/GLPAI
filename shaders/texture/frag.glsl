#version 450 core

uniform sampler2D tex;

in vec2 fragUV;

out vec4 col;

void main(){

	col = texture(tex, fragUV);

}
