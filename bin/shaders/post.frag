// A simple flat-color shader
#version 410

layout(location = 4) in vec4 gl_FragCoord;

//in vec4 vPosition;
in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform sampler2D depthTarget;
uniform int postProcessTarget;

uniform int windowWidth;
uniform int windowHeight;

uniform int blurAmount;
uniform int distortAmount;
uniform float edgeAmount;
uniform float fogAmount;

uniform float minDOF;
uniform float maxDOF;

out vec4 FragColor;

vec4 Default(vec2 texCoord) {
    return texture(colorTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord) {
    vec2 texel = 1.0f / textureSize(colorTarget, 0);
    vec4 color = texture(colorTarget, texCoord);

    for (int x = -blurAmount; x < blurAmount; x++)
    {
        for (int y = -blurAmount; y < blurAmount; y++)
        {
            color += texture(colorTarget, texCoord + texel * vec2(x, y));            
        }
    }

    color /= 1 + ((blurAmount * 2) * (blurAmount * 2));

    return color;
}

vec4 Distort(vec2 texCoord){
    vec2 mid = vec2(0.5f);
    float distanceFromCenter = distance(texCoord, mid);

    vec2 normalizeCoord = normalize(texCoord - mid);

    float bias = distanceFromCenter + sin(distanceFromCenter * distortAmount) * 0.05f;

    vec2 newCoord = mid + bias * normalizeCoord;

    return texture(colorTarget, newCoord);
}

vec4 EdgeDetection(vec2 texCoord) {
    float w = 1.0f / windowWidth;
    float h = 1.0f / windowHeight;

    vec4 k[9];

    k[0] = texture(colorTarget, texCoord + vec2(-w, -h));
    k[1] = texture(colorTarget, texCoord + vec2( 0, -h));
    k[2] = texture(colorTarget, texCoord + vec2( w, -h));
    k[3] = texture(colorTarget, texCoord + vec2(-w,  0));
    k[4] = texture(colorTarget, texCoord);
    k[5] = texture(colorTarget, texCoord + vec2( w,  0));
    k[6] = texture(colorTarget, texCoord + vec2(-w,  h));
    k[7] = texture(colorTarget, texCoord + vec2( 0,  h));
    k[8] = texture(colorTarget, texCoord + vec2( w,  h));

    vec4 sobelEdgeH = k[2] + (edgeAmount * k[5]) + k[8] - (k[0] + (edgeAmount * k[3]) + k[6]);
    vec4 sobelEdgeV = k[0] + (edgeAmount * k[1]) + k[2] - (k[6] + (edgeAmount * k[7]) + k[8]);  

    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));

    return vec4(1.0f - sobel.rgb, 1.0f) * Default(texCoord);
}

vec4 Sepia(vec2 texCoord) {
    vec4 color = texture(colorTarget, texCoord);

// Hardcoded values and code reference found at https://www.shadertoy.com/view/3slfDl
    float rr = .3;
    float rg = .769;
    float rb = .189;
    
    float gr = .3;
    float gg = .686;
    float gb = .168;
    
    float br = .272;
    float bg = .534;
    float bb = .131;

    float r = (rr * color.r) + (rg * color.g) + (rb * color.b);
    float g = (gr * color.r) + (gg * color.g) + (gb * color.b);
    float b = (br * color.r) + (bg * color.g) + (bb * color.b);

    return vec4(r, g, b, 1.0f);
}

vec4 GreyScale(vec2 texCoord) {
    vec4 color = texture(colorTarget, texCoord);

    float rr = .3;
    float rg = .3;
    float rb = .3;
    
    float gr = .3;
    float gg = .3;
    float gb = .3;
    
    float br = .3;
    float bg = .3;
    float bb = .3;

    float r = (rr * color.r) + (rg * color.g) + (rb * color.b);
    float g = (gr * color.r) + (gg * color.g) + (gb * color.b);
    float b = (br * color.r) + (bg * color.g) + (bb * color.b);

    return vec4(r, g, b, 1.0f);
}

vec4 Invert(vec2 texCoord) {
    return vec4(1.0f - texture(colorTarget, texCoord).rgb, 1.0f);
}

vec4 Pixelizer(vec2 texCoord) {
    float pixelSize = 15.0f;

    float dx = pixelSize * (1.0f/windowWidth);
    float dy = pixelSize * (1.0f/windowHeight);

    vec2 coord = vec2(dx * floor(texCoord.x/dx),
                      dy * floor(texCoord.y/dy));

    return texture(colorTarget, coord);
}

vec4 Posterize(vec2 texCoord) {
    float gamma = 0.7f;
    float numColors = 7.0f;

    vec3 color = texture(colorTarget, texCoord).xyz;

    color = pow(color, vec3(gamma));
    color = color * numColors;
    color = floor(color);
    color = color / numColors;
    color = pow(color, vec3(1.0f/gamma));

    return vec4(color, 1.0f);
}

vec4 SimpleDepthFog(vec2 texCoord) {
    return vec4((texture(depthTarget, texCoord)).xyz, 1.0) * fogAmount;
}

void main() 
{
    // This will calculate the texel size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Then we will adjust the coordinate
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    // Output post process effect here
    switch (postProcessTarget)
    {
        case 0: // Box Blur
        {
            FragColor = BoxBlur(texCoord);
            break;
        }
        case 1: // Distort
        {
            FragColor = Distort(texCoord);
            break;
        }
        case 2: // Edge Detection
        {
            FragColor = EdgeDetection(texCoord);
            break;
        }
        case 3: // Sepia
        {
            FragColor = Sepia(texCoord);
            break;
        }
        case 4: // Scanlines
        {
            FragColor = Default(texCoord);
            break;
        }
        case 5: // Grey Scale
        {
            FragColor = GreyScale(texCoord);
            break;
        }
        case 6: // Invert
        {
            FragColor = Invert(texCoord);
            break;
        }
        case 7: // Pixelizer
        {
            FragColor = Pixelizer(texCoord);
            break;
        }
        case 8: // Posterization
        {
            FragColor = Posterize(texCoord);
            break;
        }
        case 9: // Distance Fog
        {
            FragColor = SimpleDepthFog(texCoord) + Default(texCoord);
            break;
        }
        case 10: // Depth of Field
        {
            FragColor = Default(texCoord);
            break;
        }
        case 11: // Depth Texture
        {
            FragColor = texture(depthTarget, texCoord);
            break;
        }
        default:
        {
            FragColor = Default(texCoord);
            break;
        }
    }
}