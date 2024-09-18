#version 450 core

uniform sampler2D color_map;

uniform int color_map_width;
uniform int color_map_height;

in vec2 fragUV;
out vec4 col;

mat3 kernel = {
	vec3(-1.0, -1.0, -1.0),
	vec3(-1.0,  8.0, -1.0),
	vec3(-1.0, -1.0, -1.0)
};

mat3 kernel_normal = {
	vec3(0, 0, 0),
	vec3(0,  1.0, 0),
	vec3(0, 0, 0)
};

void main(){
	col = vec4(0,0,0,0);

	for(int i = -1; i < 2; i++){
		for(int k = -1; k < 2; k++){
			col += kernel_normal[k+1][i+1] * texelFetch(color_map, ivec2(fragUV * ivec2(color_map_width, color_map_height)) + ivec2(i, k), 0);
		}
	}

	col.w = 1;
}
