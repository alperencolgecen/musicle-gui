#version 430 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float blurSize;
uniform float alpha;

void main() {
    // Simple Box Blur for Glass effect (to be improved with Gaussian later)
    vec3 result = vec3(0.0);
    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
    
    int count = 0;
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            result += texture(screenTexture, TexCoords + vec2(x, y) * texelSize * blurSize).rgb;
            count++;
        }
    }
    
    FragColor = vec4(result / float(count), alpha);
}
