#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
using namespace std;

class GameState {
    private:
        bool gameOver;
        bool paused;
        bool starting;
        bool instructing;
        int distance, lives, coins, stage;
        int highScore;

    public:
        GameState();
        void reset();
        void haltStart();
        void haltInstruct();
        void pause();
        void unpause();
        void endGame();
        void updateDistance(int _distance);
        void updateHighScore(int _highscore);
        void updateLives(int _lives);
        void updateCoins(int _coins);
        void updateStage(int _stage);
        bool isPausing();
        bool isGameOver();
        bool isStarting();
        bool isInstructing();
        int currentDistance();
        int remainLives();
        int currentStage();
        int currentCoins();
        int currentHighscore();
};

#endif // GAMESTATE_HPP
