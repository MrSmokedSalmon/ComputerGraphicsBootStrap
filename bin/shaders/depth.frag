// A simple flat-color shader
#version 410

in vec4 vPosition; // World-space position

uniform vec3 CameraPosition; 

out vec4 FragColor;

// MAP FUNCTION FOUND AT https://gist.github.com/companje/29408948f1e8be54dd5733a74ca49bb9
float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main(){
    float dist = distance(CameraPosition, vPosition.xyz);

    float color = map(dist, 0.0, 20.0, 0.0, 1.0);

    FragColor = vec4( color, color, color, 1);
}