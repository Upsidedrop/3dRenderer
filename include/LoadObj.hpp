#pragma once
#include <glm/glm.hpp>

#include "Mesh.hpp"

namespace LoadObj{
    // Pushes new mesh "outMesh" to heap, delete after usage
    void ParseFile(const char* file, Mesh* outMesh);
    void gatherVertexPositions(const char* p_file, std::vector<glm::vec3>& p_vertexPositions);
    void gatherUVs(const char* p_file, std::vector<glm::vec2>& p_vertexUVs);
    void pairVertices(std::vector<GLuint>& p_indices, std::vector<glm::vec3>& p_vertexPositions, std::vector<glm::vec2>& p_vertexUVs);
}
