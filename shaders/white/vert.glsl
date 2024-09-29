#version 450 core

layout(location = 0) in vec3 pos;

uniform mat4 OVM;
uniform mat4 CVM;

void main(){

gl_Position = CVM * OVM * vec4(pos,1);
}
