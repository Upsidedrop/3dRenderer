#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <list>

#include "RenderWindow.hpp"
#include "OpenGLRenderer.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "PlayerMovement.hpp"
#include "Time.hpp"
#include "Mesh.hpp"
#include "LoadObj.hpp"

using namespace std;

OpenGLRenderer* renderer;
std::vector<Mesh*> models;
std::list<Mesh*> objects;

int main(){
    if(SDL_Init(SDL_INIT_VIDEO) > 0){
        cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << "\n";
    }

    bool gameRunning = true;
    SDL_Event event;
    RenderWindow window("RenderDemo", 800, 800);
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    renderer = new OpenGLRenderer();

    PlayerMovement player(renderer -> getCam());

    LoadObj::loadModels(
        {
            {"res/icosphere/Untitled.obj", "res/Icosphere.bmp"},
            {"res/obj/Untitled.obj","res/Cube.bmp"}
        }
    );

    models[0] -> instantiate({{0.5, 0, -0.5}, 0, {0.5, 0.5, 0.5}});
    models[1] -> instantiate({{-0.5, 0, -0.5}, 0, {0.5, 0.5, 0.5}}, {1, 0, 0, 1});
    models[1] -> instantiate({{-0.5, 1, -0.5}, 30, {0.5, 0.5, 0.5}}, {1, 1, 0, 1});
    models[1] -> instantiate({{-0.5, 2, -0.5}, 60, {0.5, 0.5, 0.5}}, {0, 1, 0, 1});

    renderer -> VertexSpecification();

    Mesh* selectedObject = nullptr;
    
    while(gameRunning){
        Time::updateDeltaTime();
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                {                
                    gameRunning = false;
                }
                break;
                case SDL_KEYDOWN:
                {
                    Input::keypresses[event.key.keysym.sym] = true;
                }
                break;
                case SDL_KEYUP:
                {
                    Input::keypresses[event.key.keysym.sym] = false;
                }
                break;
                case SDL_MOUSEMOTION:
                {
                    if(!Input::mouseButtons[2]){
                        break;
                    }
                    Input::mousePos += glm::vec2(event.motion.xrel, event.motion.yrel);
                    float limit = 89 / player.sensitivity;
                    if(Input::mousePos.y > limit){
                        Input::mousePos.y = limit;
                    }
                    if(Input::mousePos.y < -limit){
                        Input::mousePos.y = -limit;
                    }

                    player.turnCamera();
                }
                break;
                case SDL_WINDOWEVENT:
                {
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                        window.windowResized(event.window.data1,event.window.data2);
                    }
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    Input::mouseButtons[event.button.button - 1] = true;

                    if(Input::mouseButtons[2]){
                        break;
                    }
                    if(event.button.button != SDL_BUTTON_LEFT){
                        break;
                    }
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    int w, h;
                    window.getDimensions(w, h);
                    selectedObject = player.getLookAt(x, y, w, h);
                }
                break;
                case SDL_MOUSEBUTTONUP:
                {
                    Input::mouseButtons[event.button.button - 1] = false;
                }
                break;
            }
        }

        player.checkInputs();

        if(selectedObject != nullptr && Input::mouseButtons[0]){
            int x, y;
            SDL_GetMouseState(&x, &y);
            int w, h;
            window.getDimensions(w, h);
            
            auto foo = player.getPointOnPlane(selectedObject -> transform.position, glm::vec3(0, 0, 1), x, y, w, h);
            std::cout << "x: " << foo.x << ", y: " << foo.y << ", z: " << foo.z << "\n";
            selectedObject -> transform.position = foo;
        }

        {
            int outWidth, outHeight;
            window.getDimensions(outWidth, outHeight);
            if(outWidth <= 0 || outHeight <= 0){
                continue;
            }
            glViewport(0,0,outWidth,outHeight);
            renderer -> PreDraw(outWidth, outHeight);
        }
        for(Mesh* object : objects){
            object -> Draw();
        }
        window.swapWindow();
    }

    SDL_Quit();
    window.cleanUp();
    delete renderer;
    
    for(Mesh*& model : models){
        delete model;
    }
    
    return 0;
}