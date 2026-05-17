#pragma once

#include "Camera.hpp"
#include "glm/glm.hpp"

class PlayerMovement{
    public:
    PlayerMovement(Camera* p_cam);
    void checkInputs();
    void turnCamera();
    float sensitivity;
    private:
    Camera* cam;
    float speed, defaultSpeed;
    void moveDirection(glm::vec2 p_dir);
};