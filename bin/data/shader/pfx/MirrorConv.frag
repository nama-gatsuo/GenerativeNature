#version 410

uniform sampler2DRect tex0;
uniform vec2 res;
uniform int mode;

in vec2 vTexCoord;
out vec4 outputColor;

void main() {

    vec2 p = (vTexCoord * 2.0 - res) / min(res.x, res.y);
    
    if (mode == 0) {
        if (p.x > 0.0) p.x = - p.x;
    } else if (mode == 1) {
        if (p.y > 0.0) p.y = - p.y;
    }
    
    vec2 newTexCoord = (min(res.x, res.y) * p + res) * 0.5;
    
    vec4 col = texture(tex0, newTexCoord);
    
    outputColor = col;
}
