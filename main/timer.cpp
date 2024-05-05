#include "timer.hpp"
#include <SDL.h>

Timer::Timer(){
    this->startTime = 0;
    this->lastPause = 0;
    this->lastUnpause = 0;
    this->freezeTime = 0;
    this->isPausing = true;
}

void Timer::start(){
    this->startTime = SDL_GetTicks();
    this->freezeTime = 0;
    this->isPausing = false;
}

void Timer::pause(){
    if (!this->isPausing){
        this->lastPause = SDL_GetTicks();
        this->isPausing = true;
    }
}

void Timer::unpause(){
    if (this->isPausing){
        this->lastUnpause = SDL_GetTicks();
        freezeTime += (this->lastUnpause - this->lastPause);
        this->lastPause = this->lastUnpause;
        this->isPausing = false;
    }
}

int Timer::elapsedTime(){
    if (this->isPausing) return this->lastPause - this->startTime - this->freezeTime;
    else return (SDL_GetTicks() - this->startTime - this->freezeTime);
}
