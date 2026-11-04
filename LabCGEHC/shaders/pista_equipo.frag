
#version 330 core
out vec4 finalColor;

in vec2 uvData;
in vec3 normData;
in vec3 fragWorldPos;

void main() {
    vec3 lDir = normalize(vec3(0.5, 1.0, 0.4));
    float nDotL = max(dot(normalize(normData), lDir), 0.25);
    
    vec3 albedo;
    if (uvData.x >= 0.19 && uvData.x <= 0.81) {
        albedo = vec3(0.20, 0.22, 0.25);
        if (abs(uvData.x - 0.5) < 0.015 && fract(uvData.y * 0.5) < 0.5) {
            albedo = vec3(0.95, 0.80, 0.15); 
        }
    } else {
        float st = step(0.5, fract(uvData.y * 0.5));
        albedo = mix(vec3(0.85, 0.15, 0.15), vec3(0.92, 0.92, 0.92), st);
    }
    finalColor = vec4(albedo * nDotL, 1.0);
}
