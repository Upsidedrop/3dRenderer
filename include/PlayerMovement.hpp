#pragma once

#include "Camera.hpp"
#include "glm/glm.hpp"

class PlayerMovement{
    public:
    PlayerMovement(Camera* p_cam);
    void checkInputs();
    void turnCamera();
    private:
    Camera* cam;
    float speed;
    float sensitivity;
    void moveDirection(glm::vec2 p_dir);
};