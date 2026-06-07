#pragma once

#include <list>
#include <SDL2/SDL.h>

#include "Camera.hpp"
#include "glm/glm.hpp"
#include "Mesh.hpp"

extern std::list<Mesh*> objects;

class PlayerMovement{
    public:
    PlayerMovement(Camera* p_cam);
    void checkInputs();
    void turnCamera();
    float sensitivity;
    Mesh* getLookAt(int mouseX, int mouseY, int p_screenWidth, int p_screenHeight);
    glm::vec3 getPointOnPlane(const glm::vec3& offset, const glm::vec3& normal, int mouseX, int mouseY, int p_screenWidth, int p_screenHeight);
    private:
    Camera* cam;
    float speed, defaultSpeed;
    void moveDirection(glm::vec2 p_dir);
    bool checkBounds(BoundingBox& bounds, const glm::mat4& matrix, const glm::vec2& mousePos);
    bool checkQuad(glm::vec2** quad, const glm::vec2& point);
    void flattenPoints(const BoundingBox& bounds, const glm::mat4& matrix, glm::vec2* outPoints);
    void flattenVec4(const glm::vec4& vector, glm::vec2& outPoint);
    glm::vec2 pointToWindowSpace(int p_x, int p_y, int p_screenWidth, int p_screenHeight);
};