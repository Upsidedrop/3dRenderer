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

    // auto foo = perspectiveMatrix * cameraMatrix * vec4(0.5, 0, -0.5, 1);
    // std::cout << foo.x / foo.w << ", " << foo.y / foo.w <<", " << foo.z / foo.w << "\n";
    // std::cout << ((float)mouseX / float(p_screenWidth)) * 2 - 1 << ", " << 1 - ((float)mouseY / (float)p_screenHeight)*2 << "\n";

    for(Mesh* obj : objects){
        // checks whether object is between closest other object and camera
        vec4 screenPos = vec4(obj -> transform.position, 1);
        screenPos = cameraMatrix * screenPos;

        std::cout << "x: " << screenPos.x << ", y: " << screenPos.y << ", z: " << screenPos.z << ", w: " << screenPos.w << "\n";

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
        if(checkBounds(obj -> bounds, combinedMatrix, mousePos, false) || checkBounds(obj -> bounds, combinedMatrix, mousePos, true)){
            closest = obj;
            closestScreenPos = screenPos.z;
        }
    }
    return closest;
}
bool PlayerMovement::checkBounds(BoundingBox bounds, const mat4& matrix, const SDL_FPoint& mousePos, bool flipped){
    SDL_FRect bounds2d;

    if(flipped){
        std::swap<float>(bounds.smallCorner.z, bounds.largeCorner.z);
    }

    vec4 smallCorner, largeCorner;
    smallCorner = matrix * vec4(bounds.smallCorner, 1);
    largeCorner = matrix * vec4(bounds.largeCorner, 1);

    // if(smallCorner.x / smallCorner.w > largeCorner.x / largeCorner.w){
    //     std::swap<vec4>(smallCorner, largeCorner);
    // }

    SDL_FPoint points[2] = 
    {
        SDL_FPoint{smallCorner.x / smallCorner.w, smallCorner.y / smallCorner.w},
        SDL_FPoint{largeCorner.x / largeCorner.w, largeCorner.y / largeCorner.w}
    };
    SDL_EncloseFPoints(
        points,
        2, 
        NULL, 
        &bounds2d
    );
    
    // bounds2d = {
    //     smallCorner.x / smallCorner.w,
    //     smallCorner.y / smallCorner.w, 
    //     largeCorner.x / largeCorner.w - smallCorner.x / smallCorner.w,
    //     largeCorner.y / largeCorner.w - smallCorner.y / smallCorner.w
    // };

    return SDL_PointInFRect(&mousePos, &bounds2d);
}