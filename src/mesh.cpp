#include "Mesh.hpp"

#include "Utils.hpp"
#include "OpenGLRenderer.hpp"

#include <iostream>

using namespace std;

extern OpenGLRenderer* renderer;

Mesh::Mesh(vector<GLfloat>& p_points, vector<GLuint>& pointOrder, const char* p_texture)
:texture(renderer -> loadTexture(p_texture))
{
    int offset = renderer -> pushQueue(p_points, pointOrder.size());

    for(GLuint& elem : pointOrder){
        elem += offset;
    }

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::getSize<GLuint>(pointOrder.size()), pointOrder.data(), GL_STATIC_DRAW);
}
void Mesh::Draw(){
    renderer -> setTransformUniform(transform);
    renderer -> setTextureUniform(texture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    renderer -> Draw();
}
void Mesh::instantiate(const Transform& p_transform){
    Mesh* foo = new Mesh(*this);
    foo -> transform = p_transform;
    objects.push_back(foo);
}