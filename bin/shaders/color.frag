// A simple flat-color shader
#version 410

out vec4 FragColor;

uniform vec4 BaseColor;
uniform float Time;

void main(){
    vec4 newColor = BaseColor;

    float time = sin(Time * 10);
    newColor = vec4(newColor.xyz * time, 1);

    FragColor = newColor;
}