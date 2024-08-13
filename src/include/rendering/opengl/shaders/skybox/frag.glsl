#version 450 core

in vec3 fragPos;

out vec4 col;

uniform samplerCube cubemap;

void main() {
    col = vec4(texture(cubemap, vec3(-fragPos.x, fragPos.y, fragPos.z)).xyz, 1);
}
