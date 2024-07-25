#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPosition;
layout(location = 3) in vec3 fragNormal;
layout(location = 4) in vec4 cameraPos;

layout(location = 0) out vec4 outColor;


void main() {
    vec3 norm = normalize(fragNormal);

    vec3 lightPos = vec3(128, 128, 128);//cameraPos.xyz;

    vec3 lightDir = normalize(lightPos - fragPosition);

    float distance = length(lightPos - fragPosition);

    float attenuation = 1.0 / (1 + .014 * distance + 0.0007 * (distance * distance));

    float diff = max(dot(norm, lightDir), 0.0);

    diff *= attenuation;
    vec3 diffuse = diff * vec3(1);

    outColor = vec4(diffuse,1);//vec4(fragNormal, 1);//vec4(fragColor * texture(texSampler, fragTexCoord).rgb, 1.0);
}