#pragma once

#include <glm/glm.hpp>
#include <vector>

struct BoundingBox
{
    glm::vec3 smallCorner;
    glm::vec3 largeCorner;
    void generateFromPoints(std::vector<glm::vec3>& p_vertexPositions);
};
