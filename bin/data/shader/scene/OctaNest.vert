#version 400
uniform mat4 modelViewMatrix; // oF Default
uniform mat4 modelViewProjectionMatrix; // oF Default
uniform mat4 normalMatrix; // Pass from C++
in vec4 position; // oF Default

uniform float farClip;
uniform float nearClip;
uniform vec2 size;
uniform float angleFactor;
uniform float scaleFactor;
uniform float phaseFactor;
out vec4 vPosition;
out vec4 vViewPos;
out float vDepth;

void rotate(inout vec3 v, in vec3 axis, float angle){
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    mat3 m = mat3(
        oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c
    );
    v = m * v;
}

void scale(inout vec3 v, in vec3 s){
    mat3 m = mat3(
        s.x, 0.0, 0.0,
        0.0, s.y, 0.0,
        0.0, 0.0, s.z
    );
    v = m * v;
}

void trans(inout vec3 v, in vec3 t){
    v += t;
}

const float PI = 3.14159265359;

void main(){
    // gl_Position = modelViewProjectionMatrix * position;

    vec3 p = position.xyz;
    float instanceX = mod(gl_InstanceID, size.x);
    float instanceY = floor(gl_InstanceID / size.x);

    scale(p, vec3(0.5 + sin(scaleFactor / size.y * instanceY + phaseFactor) * 1.5));
    trans(p, vec3(0, 0.05 * instanceY, 0));
    rotate(p, vec3(0,0,1), PI * angleFactor / size.y * instanceY);

    rotate(p, vec3(0,1,0), PI * 2. / size.x * instanceX);
    scale(p, vec3(500.));

    vec4 viewPos = modelViewMatrix * vec4(p,1.0);
    vDepth = - viewPos.z / (farClip - nearClip);
    vPosition = modelViewProjectionMatrix * vec4(p,1.0);
    vViewPos = viewPos;
}
