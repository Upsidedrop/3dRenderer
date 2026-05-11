#pragma once

#include <glad/glad.h>
#include <vector>

#include "Transform.hpp"

class Mesh{
    public:
    Mesh(std::vector<GLfloat>& p_points, std::vector<GLuint>& pointOrder, Transform& p_transform, const char* p_texture);
    void Draw();
    private:
    GLuint indexBufferObject;
    Transform transform;
    GLuint texture;
};