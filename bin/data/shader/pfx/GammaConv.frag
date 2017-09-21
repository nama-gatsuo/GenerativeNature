#version 410

uniform sampler2DRect tex0;

in vec4 vColor;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {
    
    vec4 col = texture(tex0, vTexCoord);
    col.rgb = pow(col.rgb, vec3(1.0/2.2));
    
    outputColor = col;
}
