#include "car.hpp"

Car::Car(GameWindow *gw, float x, float y, float velocity){
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, CAR_WIDTH, CAR_HEIGHT};
    this->mVelY = velocity;
    this->mTiltedAngle = 0;
}

void Car::reset(){
    this->mRect = {SCREEN_WIDTH / 2 - CAR_WIDTH / 2, SCREEN_HEIGHT - 2 * CAR_HEIGHT, CAR_WIDTH, CAR_HEIGHT};
    this->mVelY = 0;
    this->mTiltedAngle = 0;
}

void Car::moveWithMouse(){
    int mX, mY;
    SDL_GetMouseState(&mX, &mY);
    moveTo(mX - CAR_WIDTH / 2, SCREEN_HEIGHT - 2 * CAR_HEIGHT);
}

void Car::moveTo(float x, float y){
    // Make sure the car can not move out of the roadsides
    if (x < ROADSIDE_WIDTH) x = ROADSIDE_WIDTH;
    if (x + CAR_WIDTH > SCREEN_WIDTH - ROADSIDE_WIDTH) x = SCREEN_WIDTH - ROADSIDE_WIDTH - CAR_WIDTH;

    // Create a tilt angle when moving the car to the left or right
    int dx = x - this->mRect.x;
    int dy = y - this->mRect.y;

    this->mTiltedAngle = atan(MOVEMENT_DELAY * dx / CAR_HEIGHT) * 180 / M_PI;
    if (this->mTiltedAngle >=  10) this->mTiltedAngle =  10;
    if (this->mTiltedAngle <= -10) this->mTiltedAngle = -10;

    // Delay the car when moving to both sides
    this->mRect.x += MOVEMENT_DELAY * dx;
    this->mRect.y += MOVEMENT_DELAY * dy;
}

void Car::render(SDL_Texture *tex){
    this->gwin->blit(tex, this->mRect, this->mTiltedAngle);
}

void Car::setVelY(float velocity){
    this->mVelY = velocity;
}

float Car::getPosX(){
    return this->mRect.x;
}

float Car::getPosY(){
    return this->mRect.y;
}

float Car::getVelY(){
    return this->mVelY;
}

SDL_Rect Car::getRect(){
    return this->mRect;
}
