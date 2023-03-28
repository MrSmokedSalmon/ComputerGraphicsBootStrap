// Our simple shader
#version 410

layout(location = 0) in vec4 Position;

out vec4 vPosition;

uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix;
 
void main(){
    vPosition = ModelMatrix * Position;
    gl_Position = ProjectionViewModel * Position;
}