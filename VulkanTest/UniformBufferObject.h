#pragma once
#include <ext/matrix_float4x4.hpp>

struct UniformBufferObject {
    glm::mat4 view;
    glm::mat4 proj;
};

