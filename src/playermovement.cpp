#include "PlayerMovement.hpp"

#include "Input.hpp"
#include "SDL2/SDL.h"
#include "Time.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <complex>

using namespace glm;
using namespace Input;

const int PIXEL_SIZE = 37;
PlayerMovement::PlayerMovement(Camera* p_cam)
:defaultSpeed(2), sensitivity(0.1)
{
    cam = p_cam;
}
void PlayerMovement::checkInputs(){
    if(!mouseButtons[2]){
        SDL_SetRelativeMouseMode(SDL_FALSE);
        return;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    if(keypresses[SDLK_LCTRL]){
        speed = 2 * defaultSpeed;
    }
    else{
        speed = defaultSpeed;
    }

    vec2 velocity = vec2(0, 0);
    if(keypresses[SDLK_w]){
        velocity += vec2(0, -1);
    }
    if(keypresses[SDLK_a]){
        velocity += vec2(-1, 0);
    }
    if(keypresses[SDLK_s]){
        velocity += vec2(0, 1);
    }
    if(keypresses[SDLK_d]){
        velocity += vec2(1, 0);
    }
    moveDirection(velocity);
    vec3 upForce = vec3(0);
    if(keypresses[SDLK_SPACE]){
        upForce += vec3(0, speed * Time::deltaTime, 0);
    }
    if(keypresses[SDLK_LSHIFT]){
        upForce -= vec3(0, speed * Time::deltaTime, 0);
    }
    auto currentCamPos = cam -> getPosition();
    cam -> setPosition(currentCamPos + upForce);
}
void PlayerMovement::moveDirection(vec2 p_dir){
    if(length(p_dir) == 0){
        return;
    }

    std::complex<float> complexDir = {p_dir.x, p_dir.y};
    complexDir = complexDir / std::abs(complexDir);

    std::complex<float> lookDir = {-cam -> getLookDir().z, cam -> getLookDir().x};
    lookDir = lookDir / std::abs(lookDir);

    complexDir *= lookDir;

    p_dir = vec2(complexDir.real(), complexDir.imag());

    p_dir *= speed * Time::deltaTime;

    vec3 finalVel = vec3(p_dir.x, 0, p_dir.y);

    auto currentCamPos = cam -> getPosition();
    cam -> setPosition(currentCamPos + finalVel);
}
void PlayerMovement::turnCamera(){
    cam -> turnHorizontal(-glm::radians(Input::mousePos.x * sensitivity));
    cam -> turnVertical(glm::radians(Input::mousePos.y * sensitivity));
}
Mesh* PlayerMovement::getLookAt(int mouseX, int mouseY, int p_screenWidth, int p_screenHeight){
    Mesh* closest = nullptr;
    float closestScreenPos = INFINITY;
    
    mat4 cameraMatrix = cam -> getCameraMatrix();
    mat4 perspectiveMatrix = perspective<float>(radians(45.0), (float)p_screenWidth / (float)p_screenHeight, 0.1, 10);

    for(Mesh* obj : objects){
        // checks whether object is between closest other object and camera
        vec4 screenPos = vec4(obj -> transform.position, 1);
        screenPos = cameraMatrix * screenPos;

        if(closest != nullptr){
            if(screenPos.z < closestScreenPos){
                continue;
            }
        }
        if(screenPos.z > 0){
            continue;
        }

        mat4 transformMatrix = translate(mat4(1), obj -> transform.position);
        transformMatrix = rotate<float>(transformMatrix, radians(obj -> transform.rotation), vec3(0, 1, 0));
        transformMatrix = glm::scale(transformMatrix, obj -> transform.scale);

        SDL_FPoint mousePos = {((float)mouseX / float(p_screenWidth)) * 2 - 1, 1 - ((float)mouseY / (float)p_screenHeight)*2};

        mat4 combinedMatrix = perspectiveMatrix * cameraMatrix * transformMatrix;
        if(checkBounds(obj -> bounds, combinedMatrix, mousePos)){
            closest = obj;
            closestScreenPos = screenPos.z;
        }
    }
    return closest;
}
bool PlayerMovement::checkBounds(BoundingBox& bounds, const mat4& matrix, const SDL_FPoint& mousePos){
    SDL_FPoint screenPoints[8];
    flattenPoints(bounds, matrix, screenPoints);

    // Back Face
    {
        SDL_FPoint* quad[4] =
        {
            &screenPoints[0],
            &screenPoints[3],
            &screenPoints[7],
            &screenPoints[4]
        };
        if(checkQuad(quad, mousePos)){
            return true;
        }
    }
    // Left Face
    {
        SDL_FPoint* quad[4] =
        {
            &screenPoints[0],
            &screenPoints[4],
            &screenPoints[5],
            &screenPoints[1]
        };
        if(checkQuad(quad, mousePos)){
            return true;
        }
    }
    // Right Face
    {
        SDL_FPoint* quad[4] =
        {
            &screenPoints[2],
            &screenPoints[6],
            &screenPoints[7],
            &screenPoints[3]
        };
        if(checkQuad(quad, mousePos)){
            return true;
        }
    }
    // Front Face
    {
        SDL_FPoint* quad[4] =
        {
            &screenPoints[1],
            &screenPoints[5],
            &screenPoints[6],
            &screenPoints[2]
        };
        if(checkQuad(quad, mousePos)){
            return true;
        }
    }
    // Top Face
    {
        SDL_FPoint* quad[4] =
        {
            &screenPoints[4],
            &screenPoints[7],
            &screenPoints[6],
            &screenPoints[5]
        };
        if(checkQuad(quad, mousePos)){
            return true;
        }
    }
    // Bottom Face
    {
        SDL_FPoint* quad[4] =
        {
            &screenPoints[0],
            &screenPoints[1],
            &screenPoints[2],
            &screenPoints[3]
        };
        if(checkQuad(quad, mousePos)){
            return true;
        }
    }
    return false;
}
bool PlayerMovement::checkQuad(SDL_FPoint** quad, const SDL_FPoint& point){
    const int NUM_POINTS = 4;

    int pos = 0;
    int neg = 0;

    for(int i = 0; i < NUM_POINTS; ++i){
        const SDL_FPoint* a = quad[i];
        const SDL_FPoint* b = quad[(i + 1) % NUM_POINTS];

        double d = (b -> x - a -> x) * (point.y - a -> y) - (b -> y - a -> y) * (point.x - a -> x);

        ++((d > 0)? pos : neg);

        if(pos && neg){
            return false;
        }
    }
    return true;
}
void PlayerMovement::flattenPoints(const BoundingBox& bounds, const glm::mat4& matrix, SDL_FPoint* outPoints){
    flattenVec4(matrix * vec4(bounds.smallCorner.x, bounds.smallCorner.y, bounds.smallCorner.z, 1), outPoints[0]);
    flattenVec4(matrix * vec4(bounds.smallCorner.x, bounds.smallCorner.y, bounds.largeCorner.z, 1), outPoints[1]);
    flattenVec4(matrix * vec4(bounds.largeCorner.x, bounds.smallCorner.y, bounds.largeCorner.z, 1), outPoints[2]);
    flattenVec4(matrix * vec4(bounds.largeCorner.x, bounds.smallCorner.y, bounds.smallCorner.z, 1), outPoints[3]);

    flattenVec4(matrix * vec4(bounds.smallCorner.x, bounds.largeCorner.y, bounds.smallCorner.z, 1), outPoints[4]);
    flattenVec4(matrix * vec4(bounds.smallCorner.x, bounds.largeCorner.y, bounds.largeCorner.z, 1), outPoints[5]);
    flattenVec4(matrix * vec4(bounds.largeCorner.x, bounds.largeCorner.y, bounds.largeCorner.z, 1), outPoints[6]);
    flattenVec4(matrix * vec4(bounds.largeCorner.x, bounds.largeCorner.y, bounds.smallCorner.z, 1), outPoints[7]);
}
void PlayerMovement::flattenVec4(const glm::vec4& vector, SDL_FPoint& outPoint){
    outPoint = 
    {
        vector.x / vector.w,
        vector.y / vector.w
    };
}