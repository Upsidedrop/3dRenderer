#pragma once

#include <glm/glm.hpp>

class Camera{
    public:
    Camera();
    glm::mat4 getCameraMatrix();
    void setPosition(glm::vec3 p_position);
    glm::vec3 getPosition();
    void turnHorizontal(float angle);
    glm::vec3 getLookDir();
    glm::vec3 getUpDir();
    private:
    glm::vec3 position;
    glm::vec3 lookDir;
    glm::vec3 upDir;
};