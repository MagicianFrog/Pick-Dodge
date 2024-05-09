#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
using namespace std;

class GameState {
    private:
        bool gameOver;
        bool paused;
        bool starting;
        bool instructing;
        bool dfScreen;
        int distance, lives, coins, stage;
        int highScoreEasy;
        int highScoreMedium;
        int highScoreHard;

    public:
        GameState();
        void reset();
        void haltStart();
        void haltInstruct();
        void haltDfScreen();
        void pause();
        void unpause();
        void endGame();
        void updateDistance(int _distance);
        void updateHighScore(int _highscore, int _PLAY_MODE);
        void updateLives(int _lives);
        void updateCoins(int _coins);
        void updateStage(int _stage);
        bool isPausing();
        bool isGameOver();
        bool isStarting();
        bool isInstructing();
        bool isDfScreen();
        int currentDistance();
        int remainLives();
        int currentStage();
        int currentCoins();
        int currentHighScoreEasy();
        int currentHighScoreMedium();
        int currentHighScoreHard();
};

#endif // GAMESTATE_HPP
