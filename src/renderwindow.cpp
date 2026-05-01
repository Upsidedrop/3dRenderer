#include <SDL2/SDL.h>
#include <iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) :window(nullptr){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(window == nullptr){
        std::cout << "WINDOW FAILED TO INIT. ERROR: " << SDL_GetError() << std::endl; 
    }

    context = SDL_GL_CreateContext(window);

    if(!context){
        std::cout << "CONTEXT FAILED TO INIT. ERROR: " << SDL_GetError() << std::endl; 
    }
}
void RenderWindow::cleanUp(){
    SDL_DestroyWindow(window);
}
void RenderWindow::swapWindow(){
    SDL_GL_SwapWindow(window);
}
void RenderWindow::windowResized(int p_width, int p_height){
    width = p_width;
    height = p_height;
    std::cout << "Width: " << width << ", Height: " << height << "\n";
}
void RenderWindow::getDimensions(int& outWidth, int& outHeight){
    outWidth = width;
    outHeight = height;
}