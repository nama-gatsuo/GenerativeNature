#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec4 vViewPos[];
in vec4 vPosition[];
in float vDepth[];
in float vNoise[];

out vec4 gPos;
out float gDepth;
out vec3 gBC;
out float gNoise;

void main(void){

    gl_Position = vPosition[0];
    gPos = vViewPos[0];
    gDepth = vDepth[0];
    gNoise = vNoise[0];
    gBC = vec3(1,0,0);
    EmitVertex();

    gl_Position = vPosition[1];
    gPos = vViewPos[1];
    gDepth = vDepth[1];
    gNoise = vNoise[1];
    gBC = vec3(0,1,0);
    EmitVertex();

    gl_Position = vPosition[2];
    gPos = vViewPos[2];
    gDepth = vDepth[2];
    gNoise = vNoise[2];
    gBC = vec3(0,0,1);
    EmitVertex();

    EndPrimitive();

}
