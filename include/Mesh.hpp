#pragma once

#include <glad/glad.h>
#include <vector>

#include "Transform.hpp"
#include "BoundingBox.hpp"

class Mesh{
    public:
    Mesh(std::vector<GLfloat>& p_points, std::vector<GLuint>& pointOrder, const Transform& p_transform, const char* p_texture);
    void Draw();
    Transform transform;
    BoundingBox bounds;
    private:
    GLuint indexBufferObject;
    GLuint texture;
};