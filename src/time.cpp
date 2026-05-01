#include "Time.hpp"

#include "SDL2/SDL.h"

namespace Time{
    double deltaTime = 0;
    void updateDeltaTime(){
        static Uint64 now = SDL_GetPerformanceCounter();
        static Uint64 last = 0;
        
        last = now;
        now = SDL_GetPerformanceCounter();

        deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());
    }
}