#version 450 core

in vec4 fragCol;
in float fragMet;
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragUV;

out vec4 col;

uniform sampler2D tex;

//light
uniform vec3 lPos;
uniform vec3 lCol;
uniform float lStrength;

void main(){

    vec3 lVec = lPos - fragPos;

    float light_constant = dot(normalize(lVec), fragNor) / max((length(lVec) * length(lVec) / lStrength), 1.0);

    col = fragCol * vec4(lCol.xyz * light_constant, 1.0);

}