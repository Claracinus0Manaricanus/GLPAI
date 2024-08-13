#version 450 core

layout(location = 0) in vec3 pos;

uniform mat4 CVM;

out vec3 fragPos;

void main() {
    fragPos = pos;
    gl_Position = vec4((CVM * vec4(pos, 0.0)).xyzw);
}
