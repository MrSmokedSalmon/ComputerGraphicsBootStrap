// A simple flat-color shader
#version 410

out vec4 FragColor;
layout(location = 4) in vec4 gl_FragCoord;

void main(){
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;

    FragColor = vec4( x, 0, y, 1);
}