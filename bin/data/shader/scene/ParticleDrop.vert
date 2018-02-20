#version 400
uniform mat4 modelViewMatrix; // oF Default
uniform mat4 modelViewProjectionMatrix; // oF Default
// uniform mat4 normalMatrix; // Pass from C++
in vec4 position; // oF Default
// in vec4 normal; // oF Default
in vec4 color; // oF Default
in vec2 texcoord; // oF Default

uniform float farClip;
uniform float nearClip;

uniform float time;
uniform vec3 rot;

out vec4 vPosition;
out float vDepth;
out vec4 vColor;

// y: - 100 ~ 1100

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


void main() {
    vec3 p;

    // random seed
    float r = position.y;

    // t: 0 ~ 1.
    float t = mod(time, 1000.) / 1000.;
    t = mod(t + r, 1.);

    p.x = position.x;
    p.z = position.z;
    p.y = - t * 2400 + 1200;

    rotate(p, vec3(1.0, 0., 0.), rot.x);
    rotate(p, vec3(0, 1., 0.), rot.y);
    rotate(p, vec3(0, 0., 1.0), rot.z);

    vec4 viewPos = modelViewMatrix * vec4(p, 1.0);
    vDepth = - viewPos.z / (farClip - nearClip);
    vColor = vec4(1.0);
    vPosition = viewPos;

    gl_Position = modelViewProjectionMatrix * vec4(p, 1.0);
    gl_PointSize = 1000.0 / gl_Position.w;

}
