#version 450 core

in vec3 fragPos;

out vec4 col;

uniform samplerCube cubemap;

void main() {
    col = vec4((length(vec3(1, 1, 1)) - length(fragPos)).xxx, 1);
    col = vec4(texture(cubemap, fragPos).xyz, 1);
}
