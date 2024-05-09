#include "assets.hpp"
using namespace std;

vector <SDL_Texture*> backgroundTextures(2, nullptr);
SDL_Texture *carTexture = nullptr;
SDL_Texture *carInvisibleTexture = nullptr;
SDL_Texture *obstacleSpriteTexture = nullptr;
SDL_Texture *obstacleCrashedSpriteTexture = nullptr;
SDL_Texture *coinSprite = nullptr;
SDL_Texture *goldenFontTexture = nullptr;
SDL_Texture *blueFontTexture = nullptr;
SDL_Texture *whiteFontTexture = nullptr;
SDL_Texture *metalFontTexture = nullptr;
SDL_Texture *plainWhiteFontTexture = nullptr;
SDL_Texture *plainBlackFontTexture = nullptr;
SDL_Texture *heartSymbolTexture = nullptr;
SDL_Texture *frameTexture = nullptr;

Mix_Music *bgMusic = nullptr;
Mix_Chunk *buttonClick = nullptr;
Mix_Chunk *collectCoins = nullptr;
Mix_Chunk *carCrashed = nullptr;
vector <SDL_Rect> obstaclesClipRect;

void clipObstacles();
void clipGUI();

void loadMedia(GameWindow *win){
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    backgroundTextures[0]           = win->loadTexture("assets/images/roads/road_5.png");
    backgroundTextures[1]           = win->loadTexture("assets/images/roads/road_6.png");
    carTexture                      = win->loadTexture("assets/images/cars/car.png");
    obstacleSpriteTexture           = win->loadTexture("assets/images/cars/cars.png");
    obstacleCrashedSpriteTexture    = win->loadTexture("assets/images/cars/cars_crashed.png");
    coinSprite                      = win->loadTexture("assets/images/coin.png");
    goldenFontTexture               = win->loadTexture("assets/fonts/golden.png");
    blueFontTexture                 = win->loadTexture("assets/fonts/blue.png");
    whiteFontTexture                = win->loadTexture("assets/fonts/white.png");
    metalFontTexture                = win->loadTexture("assets/fonts/metal.png");
    plainWhiteFontTexture           = win->loadTexture("assets/fonts/plain_white.png");
    plainBlackFontTexture           = win->loadTexture("assets/fonts/plain_black.png");
    heartSymbolTexture              = win->loadTexture("assets/images/HUD/heart.png");
    frameTexture                    = win->loadTexture("assets/images/HUD/frame2.png");
    bgMusic                         = Mix_LoadMUS("assets/sfx/bgmusic.mp3");
    buttonClick                     = Mix_LoadWAV("assets/sfx/button_click.wav");
    collectCoins                    = Mix_LoadWAV("assets/sfx/collect_coins.wav");
    carCrashed                      = Mix_LoadWAV("assets/sfx/car_crashed.wav");
    clipObstacles();
}

void clipObstacles(){
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});
    obstaclesClipRect.push_back({71, 109, 50, 98});
    obstaclesClipRect.push_back({133, 109, 50, 98});
    obstaclesClipRect.push_back({195, 109, 50, 98});
    obstaclesClipRect.push_back({15, 335, 46, 87});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});
    obstaclesClipRect.push_back({183, 335, 46, 87});
    obstaclesClipRect.push_back({12, 435, 50, 95});
    obstaclesClipRect.push_back({78, 435, 50, 95});
    obstaclesClipRect.push_back({143, 435, 50, 95});
    obstaclesClipRect.push_back({206, 435, 50, 95});
}

void destroyTexturesAndMusics(){
    SDL_DestroyTexture(backgroundTextures[0]);
    SDL_DestroyTexture(backgroundTextures[1]);
    SDL_DestroyTexture(carTexture);
    SDL_DestroyTexture(carInvisibleTexture);
    SDL_DestroyTexture(obstacleSpriteTexture);
    SDL_DestroyTexture(obstacleCrashedSpriteTexture);
    SDL_DestroyTexture(coinSprite);
    SDL_DestroyTexture(goldenFontTexture);
    SDL_DestroyTexture(blueFontTexture);
    SDL_DestroyTexture(whiteFontTexture);
    SDL_DestroyTexture(metalFontTexture);
    SDL_DestroyTexture(plainWhiteFontTexture);
    SDL_DestroyTexture(plainBlackFontTexture);
    SDL_DestroyTexture(heartSymbolTexture);
    SDL_DestroyTexture(frameTexture);
    Mix_FreeMusic(bgMusic);
}
