#include "logic.hpp"
#include "timer.hpp"
#include "assets.hpp"
#include "background.hpp"
#include "car.hpp"
#include "obstacle.hpp"
#include "coin.hpp"
#include "hud.hpp"
#include "gamestate.hpp"
#include "instructions.hpp"
using namespace std;

SDL_Rect column[NUMBER_OF_COLUMNS];
int colVelocity[NUMBER_OF_COLUMNS];
int maxVelY = 800;
int changeVelY = 7000;
int PLAY_MODE = 1;

GameWindow win;
GameState state;
Timer frameTimer, // Timer used to get the time per frame
      veloTimer;  // Timer used to increase the background velocity

HUD hud               (&win, &state);
Background background (&win, &backgroundTextures, INIT_VELOCITY);
Car player            (&win, SCREEN_WIDTH / 2 - CAR_WIDTH / 2, SCREEN_HEIGHT - 2 * CAR_HEIGHT, 0);
deque <Obstacle>  obstacles[NUMBER_OF_COLUMNS];
deque <Coin>      coins[NUMBER_OF_COLUMNS];

void generateColumnRanges();
void updateBgVelocity();
void renderObstacles();
void updateObstacles();
void checkCollisionsWithObstacles();
void manageObstaclesMovement();
void generateObstacles();
void renderCoins();
void renderGameScreen();
void updateCoins();
void checkCollisionsWithCoins();
void manageCoinsMovement();
void generateCoins();
void transit();
void resetGame();

int main(int agrc, char* argv[]){
    srand(time(nullptr));

    win.init();
    loadMedia(&win);

    frameTimer.start();
    veloTimer.start();

    generateColumnRanges();

    // Load game's instructions: Pick & Dodge
    string Pick  = "instructions/Pick.txt";
    string Dodge = "instructions/Dodge.txt";
    loadInstructions(Pick);
    loadInstructions(Dodge);
    for (Instructions &A : ins) cout << A.name << endl << A.desc << endl << endl;


    bool quit = false;
    SDL_Event e;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_SPACE:
                        if (state.isStarting()) state.haltStart();
                        else if (!state.isStarting() && state.isInstructing()) state.haltInstruct();
                        // else if (!state.isInstructing() && state.isDfScreen()) state.haltDfScreen();
                        if (!state.isGameOver() && !state.isInstructing() && !state.isDfScreen()){
                            if (!state.isPausing()){
                                cout << "Pause!" << endl;
                                state.pause();
                            }
                            else {
                                cout << "Unpause!" << endl;
                                state.unpause();
                            }
                        }
                        Mix_PlayChannel(-1, buttonPress, 0);
                        break;
                    case SDLK_l:
                        if (state.isGameOver()) resetGame();
                        Mix_PlayChannel(-1, buttonPress, 0);
                        break;
                    case SDLK_1:
                        transit();
                        Mix_PlayChannel(-1, buttonPress, 0);
                        break;
                    case SDLK_2:
                        transit();
                        maxVelY = 900;
                        changeVelY = 6750;
                        PLAY_MODE = 2;
                        Mix_PlayChannel(-1, buttonPress, 0);
                        break;
                    case SDLK_3:
                        transit();
                        maxVelY = 1000;
                        changeVelY = 6500;
                        PLAY_MODE = 3;
                        Mix_PlayChannel(-1, buttonPress, 0);
                        break;
                }
            }
        }

        if (!state.isPausing() && !state.isGameOver()) player.moveWithMouse();

        // Render background, coins, obstacles and player's car
        win.clearRender();
        renderGameScreen();

        // Render starting screen & instructions screen
        if (state.isStarting()) hud.renderStartingScreen();
        if (state.isInstructing() && !state.isStarting()) hud.renderInstructions();
        if (state.isDfScreen() && !state.isInstructing()) hud.renderDifficultyScreen();


        // Pause the game or finish the game
        else if (!state.isStarting() && !state.isInstructing() && !state.isDfScreen()){
            if (state.isPausing())  hud.renderPauseScreen();
            if (state.isGameOver()){
                state.updateHighScore(state.currentCoins(), PLAY_MODE);
                hud.renderGameOverScreen(PLAY_MODE);
                Mix_HaltMusic();
            }
        }

        win.presentRender();

        // Update background, obstacles and coins
        if (!state.isPausing() && !state.isGameOver()){
            background.update(frameTimer.elapsedTime() / 1000.f);
            updateBgVelocity();
            updateObstacles();
            updateCoins();
            state.updateDistance(state.currentDistance() + background.getVelY() / 60);
        }
        frameTimer.start();
    }
    destroyTextures();
    destroySounds();
    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
    return 0;
}

// Stop rendering difficulty screen and start the game + music
void transit(){
    if (!state.isInstructing() && state.isDfScreen()){
        state.haltDfScreen();
        state.unpause();
    }
    if (!Mix_PlayingMusic()) Mix_PlayMusic(bgMusic, -1);
}

void resetGame(){
    state.reset();
    background.reset(state.currentStage());
    player.reset();
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        while (!obstacles[i].empty()) obstacles[i].pop_back();
        while (!coins[i].empty())     coins[i].pop_back();
    }
    veloTimer.start();
}

void renderGameScreen(){
    // Render background, coins, obstacles and player's car
    background.render();
    renderCoins();
    renderObstacles();
    player.render(carTexture);

    // Render current scores, current coins and remaining lives on the screen
    hud.drawText(whiteFontTexture, to_string(state.currentDistance()), 30, 30, 8, 8, 3.0f, HUD_FLOAT_RIGHT);
    hud.drawText(goldenFontTexture, to_string(state.currentCoins()), 40, 65, 8, 8, 2.5f, HUD_FLOAT_RIGHT);
    hud.drawHearts(heartSymbolTexture, 30, 30, state.remainLives(), 2.0f, HUD_FLOAT_LEFT);
}

// Create the gap between columns
void generateColumnRanges(){
    int gap = (SCREEN_WIDTH - OBSTACLE_WIDTH * NUMBER_OF_COLUMNS - 2 * ROADSIDE_WIDTH) / (NUMBER_OF_COLUMNS + 1);
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        column[i].x = ROADSIDE_WIDTH + OBSTACLE_WIDTH * i + gap * (i + 1);
        column[i].w = OBSTACLE_WIDTH;
    }
}

void renderObstacles(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        for (Obstacle &X : obstacles[i]){
            X.render(X.isCrashed() ? obstacleCrashedSpriteTexture : obstacleSpriteTexture);
        }
    }
}

// Increase the velocity after every (changeVelY) ms
void updateBgVelocity(){
    if (veloTimer.elapsedTime() >= changeVelY && background.getVelY() < maxVelY){
        state.updateStage(state.currentStage() + 1);
        background.setVelY(background.getVelY() + 30);
        veloTimer.start();
    }
}

void updateObstacles(){
    checkCollisionsWithObstacles();
    manageObstaclesMovement();
    generateObstacles();
}

void checkCollisionsWithObstacles(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        for (Obstacle &X : obstacles[i]){
            // Set the velocity of the obstacles after being crashed
            if (!X.isCrashed() && checkCollision(player.getRect(), X.getRect())){
                X.crash();
                X.setVelY(background.getVelY());
                state.updateLives(state.remainLives() - 1);
                cout << "Crashed!" << endl;
                Mix_PlayChannel(-1, carCrashed, 0);
            }
        }
    }
}

void manageObstaclesMovement(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        // Move the obstacles
        for (Obstacle &X : obstacles[i]) X.setPos(column[i].x, X.getPosY() + X.getVelY() * frameTimer.elapsedTime() / 1000.f);

        // Remove all the off-screen obstacles
        while (!obstacles[i].empty() && obstacles[i].front().getPosY() >= SCREEN_HEIGHT) obstacles[i].pop_front();

    }
}

void generateObstacles(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        // Generate obstacles when there are no obstacles on the column, or when there is enough space for the car to sneak through 2 obstacles
        if (obstacles[i].empty() || obstacles[i].back().getPosY() > CAR_HEIGHT + 5){
            if (rand() % 300) continue;
            if (obstacles[i].empty()){
                // 50%: obstacle goes up, 50%: obstacle goes down
                int randomVelocity = rand() % ((state.currentStage() * 60) / 3) + 60;
                colVelocity[i] = (rand() % 2 ? randomVelocity : 0 - randomVelocity);
            }
            Obstacle newObstacle (
                &win,
                column[i].x,
                -OBSTACLE_HEIGHT,
                background.getVelY() + colVelocity[i],
                obstaclesClipRect[rand() % (int) obstaclesClipRect.size()]
            );
            obstacles[i].push_back(newObstacle);
        }
    }
}

void renderCoins(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        for (Coin& C : coins[i]){
            if (!C.isClaimed()) C.render(coinSprite);
        }
    }
}

void updateCoins(){
    checkCollisionsWithCoins();
    manageCoinsMovement();
    generateCoins();
}

void checkCollisionsWithCoins(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        for (Coin& C : coins[i]){
            if (!C.isClaimed() && checkCollision(player.getRect(), C.getRect())){
                C.claimed();
                state.updateCoins(state.currentCoins() + 1);
                Mix_PlayChannel(-1, collectCoins, 0);
            }
        }
    }
}

void manageCoinsMovement(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        // Use 'gap' to put the coins in the center of the road column
        for (Coin& C : coins[i]){
            float gap = (column[i].w - COIN_WIDTH) / 2;
            C.setPos(column[i].x + gap, C.posY() + background.getVelY() * frameTimer.elapsedTime() / 1000.f);
            C.animate();
        }

        // Remove all the off-screen coins
        while (!coins[i].empty() && coins[i].front().posY() >= SCREEN_HEIGHT){
            coins[i].pop_front();
        }
    }
}

void generateCoins(){
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++){
        if (coins[i].empty()){
            // The probability of dropping coins on the road column
            if (rand() % 500) continue;
            int numberOfCoins = rand() % 6 + 5;
            int gap = 25;
            float current_y = 0;

            // Create coins's chain for the current road column
            while (numberOfCoins--){
                current_y -= COIN_HEIGHT;
                Coin C(&win, column[i].x, current_y);
                coins[i].push_back(C);
                current_y -= gap;
            }
        }
    }
}
