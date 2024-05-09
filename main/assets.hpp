#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "gamewindow.hpp"
using namespace std;

extern vector <SDL_Texture*> backgroundTextures;
extern SDL_Texture *carTexture;
extern SDL_Texture *obstacleSpriteTexture;
extern SDL_Texture *obstacleCrashedSpriteTexture;
extern SDL_Texture *coinSprite;
extern SDL_Texture *goldenFontTexture;
extern SDL_Texture *blueFontTexture;
extern SDL_Texture *whiteFontTexture;
extern SDL_Texture *metalFontTexture;
extern SDL_Texture *plainWhiteFontTexture;
extern SDL_Texture *plainBlackFontTexture;
extern SDL_Texture *heartSymbolTexture;
extern SDL_Texture *frameTexture;
extern vector <SDL_Rect> obstaclesClipRect;

extern Mix_Music *bgMusic;

void loadMedia(GameWindow *win);
void destroyTexturesAndMusics();

#endif // ASSETS_HPP
