#version 430 core

struct ParticleData {
    vec2 pos;
    vec2 prevPos;
    vec2 acc;
    float mass;
    float padding;
};

layout(std430, binding = 0) buffer ParticleBuffer {
    ParticleData particles[];
};

uniform vec2 screenRes;
out float vIntensity;

void main() {
    ParticleData p = particles[gl_InstanceID];
    vec2 pos = p.pos;
    
    // Calculate speed for intensity
    float speed = length(p.pos - p.prevPos);
    vIntensity = clamp(speed * 5.0, 0.0, 1.0);
    
    // Convert screen coordinates [0, Res] to NDC [-1, 1]
    vec2 ndcPos = (pos / screenRes) * 2.0 - 1.0;
    
    gl_Position = vec4(ndcPos, 0.0, 1.0);
    
    // Dynamic point size based on speed
    gl_PointSize = 4.0 + speed * 2.0;
}
