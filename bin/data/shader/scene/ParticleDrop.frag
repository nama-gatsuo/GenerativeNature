#version 400

in vec4 vPosition;
in float vDepth;
in vec4 vColor;

layout (location = 0) out vec4 outputColor0;
layout (location = 1) out vec4 outputColor1;
layout (location = 2) out vec4 outputColor2;
layout (location = 3) out vec4 outputColor3;

void main() {

    vec3 n;
    n.xy = gl_PointCoord.xy * 2.0 - 1.0;
    n.z = 1.0 - dot(n.xy, n.xy);

    float pAlpha = smoothstep(0.0, 1.0, n.z);

    outputColor0 = vec4(vec3(1.1, 0.8, 0.9), 1.);
    outputColor1 = vPosition;
    outputColor2 = vec4(vec3(0.), vDepth);
    outputColor3 = vec4(vec3(1.1, 0.8, 0.9), pAlpha);
}
