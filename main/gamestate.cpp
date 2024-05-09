#include "gamestate.hpp"
#include "params.hpp"
#include <fstream>
#include <iostream>
#define HIGHSCORE_DATA_FILE_EASY "game_data/highScoreEasy.txt"
#define HIGHSCORE_DATA_FILE_MEDIUM "game_data/highScoreMedium.txt"
#define HIGHSCORE_DATA_FILE_HARD "game_data/highScoreHard.txt"
using namespace std;

int getHighScoreFromDataFile(string path){
    int score = 0;
    ifstream file(path);
    if (!file.is_open()){
        clog << "Failed to open: " << path << endl;
        return {0};
    }
    string s;
    file >> s;
    for (const char &c : s) score = score * 10 + int(c - '0');
    file.close();
    return score;
}

void updateHighScoreToDataFile(string path, int &highScore){
    ofstream file(path);
    if (!file.is_open()){
        cout << "Failed to open: " << path << endl;
        return;
    }
    // Clear the file by setting the position indicator to the beginning
    file.seekp(0, ios::beg);
    file << highScore << endl;
    file.close();
}

GameState::GameState(){
    reset();
}

void GameState::reset(){
    this->distance = 0;
    this->lives = NUMBER_OF_LIVES;
    this->coins = 0;
    this->stage = 1;
    this->paused = 1;
    this->starting = 1;
    this->instructing = 1;
    this->dfScreen = 1;
    this->gameOver = 0;
    this->highScoreEasy = getHighScoreFromDataFile(HIGHSCORE_DATA_FILE_EASY);
    this->highScoreMedium = getHighScoreFromDataFile(HIGHSCORE_DATA_FILE_MEDIUM);
    this->highScoreHard = getHighScoreFromDataFile(HIGHSCORE_DATA_FILE_HARD);
}

void GameState::pause(){
    this->paused = 1;
}

void GameState::unpause(){
    this->paused = 0;
}

void GameState::endGame(){
    this->gameOver = 1;
}

void GameState::haltStart(){
    this->starting = 0;
}

void GameState::haltInstruct(){
    this->instructing = 0;
}

void GameState::haltDfScreen(){
    this->dfScreen = 0;
}

bool GameState::isStarting(){
    return this->starting;
}

bool GameState::isInstructing(){
    return this->instructing;
}

bool GameState::isDfScreen(){
    return this->dfScreen;
}

void GameState::updateDistance(int _distance){
    this->distance = _distance;
}

void GameState::updateLives(int _lives){
    if (gameOver) return;
    this->lives = _lives;
    if (this->lives == 0) {
        this->endGame();
    }
}

void GameState::updateStage(int _stage){
    if (gameOver) return;
    this->stage = _stage;
}

void GameState::updateCoins(int _coin){
    this->coins = _coin;
}

void GameState::updateHighScore(int _highscore, int _PLAY_MODE){
    if (gameOver){
        if (_PLAY_MODE == 1){
            if (this->highScoreEasy < _highscore){
                this->highScoreEasy = _highscore;
                updateHighScoreToDataFile(HIGHSCORE_DATA_FILE_EASY, this->highScoreEasy);
            }
        }
        else if (_PLAY_MODE == 2){
            if (this->highScoreMedium < _highscore){
                this->highScoreMedium = _highscore;
                updateHighScoreToDataFile(HIGHSCORE_DATA_FILE_MEDIUM, this->highScoreMedium);
            }
        }
        else if (_PLAY_MODE == 3){
            if (this->highScoreHard < _highscore){
                this->highScoreHard = _highscore;
                updateHighScoreToDataFile(HIGHSCORE_DATA_FILE_HARD, this->highScoreHard);
            }
        }
    }
}

bool GameState::isPausing(){
    return this->paused;
}

bool GameState::isGameOver(){
    return this->gameOver;
}

int GameState::currentDistance(){
    return this->distance;
}

int GameState::remainLives(){
    return this->lives;
}

int GameState::currentCoins(){
    return this->coins;
}

int GameState::currentStage(){
    return this->stage;
}

int GameState::currentHighScoreEasy(){
    return this->highScoreEasy;
}

int GameState::currentHighScoreMedium(){
    return this->highScoreMedium;
}

int GameState::currentHighScoreHard(){
    return this->highScoreHard;
}
