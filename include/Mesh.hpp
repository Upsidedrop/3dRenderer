#pragma once

#include <glad/glad.h>
#include <vector>
#include <list>
#include <glm/glm.hpp>

#include "Transform.hpp"

class Mesh;

extern std::list<Mesh*> objects;

class Mesh{
    public:
    Mesh(std::vector<GLfloat>& p_points, std::vector<GLuint>& pointOrder, const char* p_texture);
    void Draw();
    Transform transform;
    void instantiate(const Transform& p_transform, const glm::vec4& p_color = glm::vec4(1));
    private:
    GLuint indexBufferObject;
    GLuint texture;
    glm::vec4 color;
};