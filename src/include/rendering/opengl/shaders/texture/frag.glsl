#version 450 core

in vec4 fragCol;
in float fragMet;
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragUV;

out vec4 col;

uniform sampler2D tex;
uniform vec3 cameraPos;
uniform float metallic;

//light
uniform vec3 lPos;
uniform vec3 lCol;
uniform float lStrength;

void main() {
    vec3 lVec = lPos - fragPos;
    vec3 cVec = cameraPos - fragPos;

    float light_constant = dot(normalize(lVec), fragNor) / max((length(lVec) * length(lVec) / lStrength), 1.0);
    float specular_constant = metallic * dot(normalize((normalize(cVec) + normalize(lVec))), fragNor) / max(length(lVec) / lStrength, 1.0);

    col = fragCol * vec4((lCol * texture(tex, fragUV).xyz * light_constant) + (lCol * vec3(pow(specular_constant, 20))), 1.0);
}
