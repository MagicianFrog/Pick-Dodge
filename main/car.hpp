#ifndef CAR_HPP
#define CAR_HPP

#include "gamewindow.hpp"

class Car {
    private:
        GameWindow *gwin = nullptr;
        SDL_Rect mRect;
        float mVelY;
        int mTiltedAngle;

    public:
        Car(){};
        Car(GameWindow *gw, float x, float y, float velocity);
        void reset();
        void moveWithMouse();
        void moveTo(float x, float y);
        void render(SDL_Texture *tex);
        void setVelY(float velocity);
        float getPosX();
        float getPosY();
        float getVelY();
        SDL_Rect getRect();
};

#endif // CAR_HPP
