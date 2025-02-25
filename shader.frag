#version 330 core

uniform sampler2D image;
uniform float kernel[49];

in vec2 texCoord;
out vec4 fragColor;

void main() {
    vec2 texelSize = 1.0 / textureSize(image, 0);
    
    vec3 sum = vec3(0.0);
    int index = 0;
    
    for(int y = -3; y <= 3; ++y) {
        for(int x = -3; x <= 3; ++x) {
            vec2 offset = vec2(x, y) * texelSize;
            sum += texture(image, texCoord + offset).rrr * kernel[index++];
        }
    }
    
    //fragColor = vec4(sum, 1.0);
    fragColor = vec4(texture(image, texCoord).rrr, 1.0);
}