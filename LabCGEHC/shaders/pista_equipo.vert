
#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNorm;

out vec2 uvData;
out vec3 normData;
out vec3 fragWorldPos;

uniform mat4 mMat;
uniform mat4 vMat;
uniform mat4 pMat;

void main() {
    fragWorldPos = vec3(mMat * vec4(inPos, 1.0));
    normData = mat3(transpose(inverse(mMat))) * inNorm;
    uvData = inUV;
    gl_Position = pMat * vMat * vec4(fragWorldPos, 1.0);
}
