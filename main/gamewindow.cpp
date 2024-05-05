#include "gamewindow.hpp"
using namespace std;

bool GameWindow::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        cout << "SDL_Init failed" << endl;
        return false;
    }
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & (imgFlags))){
        cout << "IMG_Init failed" << endl;
        return false;
    }
    gWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (gWindow == nullptr){
        cout << "SDL_CreateWindow failed" << endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr){
        cout << "SDL_CreateRenderer failed" << endl;
        return false;
    }
    return true;
}

SDL_Texture *GameWindow::loadTexture(string path){
    SDL_Texture *texture;
    texture = IMG_LoadTexture(gRenderer, path.c_str());
    if (texture == nullptr){
        cout << "Failed to load texture from " << path << ' ' << IMG_GetError() << endl;
    }
    return texture;
}

void GameWindow::blit(SDL_Texture *texture, SDL_Rect clip, SDL_Rect rect){
    SDL_Rect srect = clip;
    SDL_Rect drect = rect;
    SDL_RenderCopyEx(gRenderer, texture, &srect, &drect, 0, nullptr, SDL_FLIP_NONE);
}

void GameWindow::blit(SDL_Texture *texture, SDL_Rect rect, double angle){
    SDL_Rect drect = rect;
    SDL_RenderCopyEx(gRenderer, texture, nullptr, &drect, angle, nullptr, SDL_FLIP_NONE);
}

void GameWindow::clearRender(){
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
}

void GameWindow::presentRender(){
    SDL_RenderPresent(gRenderer);
}
