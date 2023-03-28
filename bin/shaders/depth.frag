// A simple flat-color shader
#version 410

in vec4 vPosition; // World-space position

uniform vec3 CameraPosition; 

out vec4 FragColor;

// look at https://lettier.github.io/3d-game-shaders-for-beginners/depth-of-field.html
// https://lettier.github.io/3d-game-shaders-for-beginners/ssao.html#vertex-positions

void main(){
    float dist = distance(CameraPosition, vPosition.xyz);

    color = 1.0/dist;

    FragColor = vec4( dist, dist, dist, 1);
}