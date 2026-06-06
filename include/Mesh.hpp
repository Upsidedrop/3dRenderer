#pragma once

#include <glad/glad.h>
#include <vector>
#include <list>

#include "Transform.hpp"

class Mesh;

extern std::list<Mesh*> objects;

class Mesh{
    public:
    Mesh(std::vector<GLfloat>& p_points, std::vector<GLuint>& pointOrder, const char* p_texture);
    void Draw();
    Transform transform;
    void instantiate(const Transform& p_transform);
    private:
    GLuint indexBufferObject;
    GLuint texture;
};