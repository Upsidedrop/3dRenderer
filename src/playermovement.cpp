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