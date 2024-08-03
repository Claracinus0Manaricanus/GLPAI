#version 450 core

layout(location = 0) in vec3 pos;

uniform mat4 CVM;
uniform vec4 color;

out vec3 fragPos;
out vec4 fragCol;

void main() {
    fragCol = color;
    fragPos = pos;
    gl_Position = vec4((CVM * vec4(pos, 0.0)).xyzw);
}
