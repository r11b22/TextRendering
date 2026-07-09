#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aUV;

struct InstanceData {
    mat4 uModelMatrix;
};

layout(std430, binding = 3) readonly buffer InstanceBuffer {
    InstanceData instances[];
};

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

void main()
{
    mat4 modelMatrix = instances[gl_InstanceID].uModelMatrix;

    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    gl_Position = uProjectionMatrix * uViewMatrix * vec4(FragPos, 1.0);

    Normal = mat3(transpose(inverse(modelMatrix))) * aNorm;
    UV = aUV;
}
