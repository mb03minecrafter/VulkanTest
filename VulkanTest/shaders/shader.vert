#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPosition;
layout(location = 3) out vec3 fragNormal;

layout(location = 4) out vec4 cameraPos;



//push constants block
layout( push_constant ) uniform constants
{
	mat4 model;
} PushConstants;

void main() {
    gl_Position = ubo.proj * ubo.view * PushConstants.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;


    fragNormal = normalize(inNormal * transpose(inverse(mat3(PushConstants.model))));

    fragPosition =  vec3(PushConstants.model * vec4(inPosition, 1.0));

    mat4 inverseview = inverse(ubo.view);
    cameraPos = inverseview[3];


}