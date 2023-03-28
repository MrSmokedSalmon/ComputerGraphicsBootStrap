// Our simple shader
#version 410

layout(location = 0) in vec4 Position;

out vec4 vPosition;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

void main()
{
    // Generate the texture coordinates
    vPosition = ProjectionViewModel * Position;
    vTexCoord = Position.xy * 0.5f + 0.5f;
    gl_Position = vec4(Position.xy, 0, 1);
}