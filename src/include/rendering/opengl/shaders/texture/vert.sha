#version 450 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 nor;
layout(location=2) in vec2 uv;

uniform mat4 CVM;
uniform mat4 OVM;
uniform vec4 color;
uniform float metalness;

out vec4 fragCol;
out float fragMet;
out vec3 fragPos;
out vec3 fragNor;
out vec2 fragUV;

void main(){

    fragCol = color;
    fragMet = metalness;
    fragPos = (OVM * vec4(pos.xyz, 1.0)).xyz;
    fragNor = normalize((OVM * vec4(nor.xyz, 0)).xyz);
    fragUV = uv;
    gl_Position = CVM * OVM * vec4(pos.xyz, 1.0);

    return;
}