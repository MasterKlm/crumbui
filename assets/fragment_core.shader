#version 330 core
out vec4 fragColor;
in vec4 ourColor;

uniform vec2 uRectPos;
uniform vec2 uRectSize;
uniform float uRadius;

float roundedBoxSDF(vec2 p, vec2 halfSize, float r){
    vec2 q = abs(p) - halfSize + r;
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;
}

void main(){
    vec2 center = uRectPos + uRectSize * 0.5;
    vec2 p = gl_FragCoord.xy - center;
    float d = roundedBoxSDF(p, uRectSize * 0.5, uRadius);
    if(uRadius > 0.0 && d > 0.0) discard;
    fragColor = ourColor;
}