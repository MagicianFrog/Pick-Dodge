#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "gamewindow.hpp"
using namespace std;

class Background {
    private:
        GameWindow *gwin = nullptr;
        vector <SDL_Texture*> *bgTextures;
        SDL_Rect mUpBgRect, mDownBgRect;
        int whichBg[2] = {0, 1};
        float mVelY;
        float mOffsetY;

    public:
        Background(){};
        Background(GameWindow *gw, vector <SDL_Texture*> *bgTex, float velocity);
        void reset(int stage);//
        void render();
        void update(float dTime = 0.016);
        void setVelY(float velocity);
        float getVelY();
};

#endif // BACKGROUND_HPP
