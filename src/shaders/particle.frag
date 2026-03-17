#version 430 core

in float vIntensity;
out vec4 FragColor;

void main() {
    // Distance from center of point (0.5, 0.5)
    float dist = length(gl_PointCoord - vec2(0.5));
    
    // Smooth falloff for circular glow
    float alpha = 1.0 - smoothstep(0.1, 0.5, dist);
    
    if (alpha <= 0.0) discard;
    
    // Gradient from Deep Sea Cyan to Royal Violet based on speed/mass
    // Boost intensity for faster particles
    vec3 baseColor = mix(vec3(0.0, 0.4, 0.8), vec3(0.0, 1.0, 1.0), vIntensity);
    vec3 glowColor = mix(baseColor, vec3(0.7, 0.2, 1.0), dist);
    
    FragColor = vec4(glowColor * (1.0 + vIntensity), alpha * 0.4);
}
