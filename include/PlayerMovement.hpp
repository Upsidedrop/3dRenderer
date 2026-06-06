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
    private:
    Camera* cam;
    float speed, defaultSpeed;
    void moveDirection(glm::vec2 p_dir);
    bool checkBounds(BoundingBox& bounds, const glm::mat4& matrix, const SDL_FPoint& mousePos);
    bool checkQuad(SDL_FPoint** quad, const SDL_FPoint& point);
    void flattenPoints(const BoundingBox& bounds, const glm::mat4& matrix, SDL_FPoint* outPoints);
    void flattenVec4(const glm::vec4& vector, SDL_FPoint& outPoint);
};