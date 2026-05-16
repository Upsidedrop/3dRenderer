#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>


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

    std::vector<GLfloat> vertexData{
        -0.5, -0.5, 0,
        0.0, 1.0, 0.0,

         0.5, -0.5, 0,
         1.0, 1.0, 0.0,

        -0.5,  0.5, 0,
         0.0, 0.0, 0.0,

         0.5, 0.5, 0,
         1.0, 0.0, 0.0
    };
    std::vector<GLuint> indexBufferData{
        1, 2, 0, 1, 3, 2
    };
    Transform squarePos = {{0, 0, -2}, 0, {1, 1, 1}};
    Mesh square(vertexData, indexBufferData, squarePos, "res/texture.bmp");

    Transform square2Pos = {{-2, 0, -1}, 0, {1, 1, 1}};
    Mesh square2(vertexData, indexBufferData, square2Pos, "res/texture2.bmp");

    Mesh* foo;
    LoadObj::ParseFile("res/icosphere/Untitled.obj",foo, "res/Icosphere.bmp");

    foo -> transform.scale = {0.5, 0.5, 0.5};
    foo -> transform.position = {0.5, 0, -0.5};

    Mesh* bar;
    LoadObj::ParseFile("res/obj/Untitled.obj",bar, "res/Cube.bmp");

    bar -> transform.scale = {0.5, 0.5, 0.5};
    bar -> transform.position = {-0.5, 0, -0.5};



    renderer -> VertexSpecification();

    

    SDL_SetRelativeMouseMode(SDL_TRUE);
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
                    Input::mousePos += glm::vec2(event.motion.xrel, event.motion.yrel);
                    player.turnCamera();
                }
                break;
                case SDL_WINDOWEVENT:
                {
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                        window.windowResized(event.window.data1,event.window.data2);
                    }
                }
            }
        }

        player.checkInputs();

        {
            int outWidth, outHeight;
            window.getDimensions(outWidth, outHeight);
            glViewport(0,0,outWidth,outHeight);
            renderer -> PreDraw(outWidth, outHeight);
        }
        square.Draw();
        square2.Draw();
        foo -> Draw();
        bar -> Draw();
        window.swapWindow();
    }

    SDL_Quit();
    window.cleanUp();
    delete renderer;
    delete foo;
    delete bar;
    
    return 0;
}