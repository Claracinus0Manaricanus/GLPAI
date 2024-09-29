#version 450 core

layout(location = 0) in vec3 pos;

uniform mat4 CVM;
uniform mat4 OVM;

void main(){
	gl_Position = CVM * OVM * vec4(pos,1);
}
