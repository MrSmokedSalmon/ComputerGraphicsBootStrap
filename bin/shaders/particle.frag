// A simple flat-color shader
#version 410

in vec4 vColor;

out vec4 FragColor;

void main()
{
    FragColor = vColor;
}