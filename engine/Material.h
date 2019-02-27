#pragma once

#include <glm/glm.hpp>

namespace engine 
{
    // Used by model loading code
    struct Material
    {
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 ambient;
        float shininess;
    };
}