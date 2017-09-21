#version 400
#pragma include "./noise3D.frag"
#pragma include "./random.frag"

uniform mat4 modelViewMatrix; // oF Default
uniform mat4 modelViewProjectionMatrix; // oF Default
in vec4 position; // oF Default
in vec4 normal;

uniform float farClip;
uniform float nearClip;

uniform float heightFactor;
uniform float noiseFactor;
uniform float wireWidth;
uniform float time;

out vec4 vPosition;
out vec4 vViewPos;
out float vDepth;
out float vNoise;

void main(){

    vec4 p = position;
    float noise = pow(cnoise(p.xyz * noiseFactor * 0.2 + time), 2.);

    p.xyz += normal.xyz * noise * 300. * (0.4 + heightFactor);

    vec4 viewPos = modelViewMatrix * p;
    vDepth = - viewPos.z / (farClip - nearClip);
    vPosition = modelViewProjectionMatrix * p;
    vViewPos = viewPos;
    vNoise = noise;
}
