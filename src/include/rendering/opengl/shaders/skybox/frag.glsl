#version 450 core

in vec3 fragPos;
in vec4 fragCol;

out vec4 col;

void main() {
    col = fragCol * vec4((length(vec3(1, 1, 1)) - length(fragPos)).xxx, 1);
}
