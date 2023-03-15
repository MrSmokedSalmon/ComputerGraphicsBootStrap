// Our phong shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

out vec3 vNormal;
out vec4 vPosition;

uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix; // To transform the normal

void main(){
    vNormal = (ModelMatrix * Normal).xyz;
    vPosition = ModelMatrix * Position;
    gl_Position = ProjectionViewModel * Position;
}