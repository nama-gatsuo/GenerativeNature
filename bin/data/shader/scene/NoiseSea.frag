#version 400

uniform float wireWidth;
uniform int isShadow;
uniform float colFactor;

in vec4 gPos;
in float gDepth;
in float gNoise;
in vec3 gBC;

layout (location = 0) out vec4 outputColor0;
layout (location = 1) out vec4 outputColor1;
layout (location = 2) out vec4 outputColor2;
layout (location = 3) out vec4 outputColor3;

vec3 calcFlatNormal(vec3 posInViewSpace){
    vec3 dx = dFdx(posInViewSpace);
    vec3 dy = dFdy(posInViewSpace);
    vec3 n = normalize(cross(normalize(dx), normalize(dy)));

    return - n;
}

void main(void){

    if (isShadow == 1) {

        if(any(lessThan(gBC, vec3(wireWidth)))){
            outputColor0.r = gDepth;
            outputColor0.a = 1.0;
        } else {
            discard;
        }

    } else {

        if(any(lessThan(gBC, vec3(wireWidth)))){
            vec3 col = vec3(0.3, 0.5, 1.3) * abs(gNoise) * 3.0;
            col = mix(vec3(0.0), col, colFactor);
            outputColor0 = vec4(col, 1.0);

            outputColor1 = gPos;
            outputColor2 = vec4(calcFlatNormal(gPos.xyz), gDepth);

            vec4 c = vec4(vec3(0.0), 1.0);
            if (dot(outputColor0, outputColor0) > dot(vec4(1.0), vec4(1.0))) c = outputColor0;
            outputColor3 = c;
        } else {
            discard;
        }

    }
}
