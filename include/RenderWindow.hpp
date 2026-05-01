#pragma once
#include <SDL2/SDL.h>

class RenderWindow{
    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        void cleanUp();  
        void swapWindow();
        void getDimensions(int& outWidth, int& outHeight);
        void windowResized(int p_width, int p_height);
    private:
        SDL_Window* window;
        SDL_GLContext context;
        int height, width;
};
