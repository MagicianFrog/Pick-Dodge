#include "hud.hpp"
#include "assets.hpp"
#include "instructions.hpp"
using namespace std;

HUD::HUD(GameWindow *gw, GameState *gs){
    this->gwin = gw;
    this->gstate = gs;
}

void HUD::drawText(SDL_Texture* tex, string text, float x, float y, int letterWidth, int letterHeight, float SCALE, int alignX){
    // Get the starting x-y positions of every sprite
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;

    // Set the scale of every letter on the screen
    int scaledLetterWidth = SCALE * letterWidth;
    int scaledLetterHeight = SCALE * letterHeight;

    float letterSpacing = -3;

    int totalLength = scaledLetterWidth * int(text.size());
    switch (alignX){
        case HUD_FLOAT_RIGHT:
            x = SCREEN_WIDTH - x - totalLength;
            break;
        case HUD_FLOAT_CENTER:
            x = SCREEN_WIDTH / 2 - totalLength / 2;
            break;
    }

    for (char &c : text){
        if ('a' <= c && c <= 'z') c += ('A' - 'a');
        // Get the order through the source image in "assets" file
        int asciiOrder = int(c - ' ');
        SDL_Rect srect = {
            (asciiOrder % spriteCols) * letterWidth,
            (asciiOrder / spriteCols) * letterHeight,
            letterWidth,
            letterHeight
        };
        this->gwin->blit(tex, srect, {(int)x, (int)y, scaledLetterWidth, scaledLetterHeight});
        x += scaledLetterWidth + letterSpacing;
    }
}

void HUD::drawParagraph(string text, SDL_Rect drect, SDL_Texture* tex, int letterWidth, int letterHeight, float SCALE){
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;
    int scaledLetterWidth = SCALE * letterWidth;
    int scaledLetterHeight = SCALE * letterHeight;

    float lineSpacing = -2;
    float letterSpacing = 0;

    float x = drect.x;
    float y = drect.y;
    for (int i = 0; i < (int) text.size(); i++){
        string word = "";
        int j;
        for (j = i; j < (int) text.size(); j++){
            word += text[j];
            if (text[j] == ' ') break;
        }

        // End of a line
        if (x + scaledLetterWidth * ((int) word.size() - 1) > drect.x + drect.w){
            x = drect.x;
            y += scaledLetterHeight + lineSpacing;
        }
        for (char &c : word){
            int asciiOrder = int(c - ' ');
            SDL_Rect srect = {
                (asciiOrder % spriteCols) * letterWidth,
                (asciiOrder / spriteCols) * letterHeight,
                letterWidth,
                letterHeight
            };
            this->gwin->blit(tex, srect, {(int)x, (int)y, scaledLetterWidth, scaledLetterHeight});
            x += scaledLetterWidth + letterSpacing;
        }
        i = j;
    }
}

void HUD::drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale, int alignX){
    SDL_Rect srect = {0, 0, 16, 16};
    SDL_Rect drect = {(int)x, (int)y, (int) (scale * srect.w), (int) (scale * srect.h)};
    int totalLength = drect.w * NUMBER_OF_LIVES;
    switch (alignX){
        case HUD_FLOAT_RIGHT:
            drect.x = SCREEN_WIDTH - x - totalLength;
            break;
        case HUD_FLOAT_CENTER:
            drect.x = SCREEN_WIDTH / 2 - totalLength / 2;
            break;
    }
    for (int i = 1; i <= NUMBER_OF_LIVES; i++){
        // Render the hearts by setting the starting position
        srect.x = (i <= gstate->remainLives() ? 0 : srect.w);
        this->gwin->blit(tex, srect, drect);
        drect.x += drect.w + 5;
    }
}

// Draw a 'fadePercentage' opacity black rectangle covering the screen
void HUD::drawFadeOverlay (int fadePercentage){
    SDL_SetRenderDrawColor(gwin->gRenderer, 0, 0, 0, fadePercentage * 255 / 100);
    SDL_SetRenderDrawBlendMode(gwin->gRenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect fullScreenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(gwin->gRenderer, &fullScreenRect);
}

// Render starting screen
void HUD::renderStartingScreen(){
    this->drawFadeOverlay(75);
    this->drawFadeOverlay(75);
    this->drawText(blueFontTexture, "PICK", 110, 200, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(blueFontTexture, "DODGE", 200, 250, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(whiteFontTexture, "PRESS SPACE TO CONTINUE", 100, 570, 8, 8, 2.0f, HUD_FLOAT_LEFT);
}

void HUD::renderDifficultyScreen(){
    this->drawFadeOverlay(75);
    this->drawFadeOverlay(75);
    this->drawText(blueFontTexture, "EASY", 100, 200, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(blueFontTexture, "1", 360, 200, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(blueFontTexture, "MEDIUM", 100, 250, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(blueFontTexture, "2", 360, 250, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(blueFontTexture, "HARD", 100, 300, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(blueFontTexture, "3", 360, 300, 8, 8, 5.0f, HUD_FLOAT_LEFT);
    this->drawText(whiteFontTexture, "CHOOSE DIFFICULTY", 137, 570, 8, 8, 2.0f, HUD_FLOAT_LEFT);
}

void HUD::renderGameOverScreen(int _PLAY_MODE){
    int currentHighScore;
    if (_PLAY_MODE == 1) currentHighScore = gstate->currentHighScoreEasy();
    else if (_PLAY_MODE == 2) currentHighScore = gstate->currentHighScoreMedium();
    else if (_PLAY_MODE == 3) currentHighScore = gstate->currentHighScoreHard();
    this->drawFadeOverlay(75);
    this->drawText(metalFontTexture, "GAMEOVER!", 100, 160, 64, 64, 0.6f, HUD_FLOAT_LEFT);
    if (gstate->currentCoins() >= currentHighScore){
        this->drawText(whiteFontTexture, "NEW HIGHSCORE!", 110, 260, 8, 8, 3.0f, HUD_FLOAT_LEFT);
        this->drawText(goldenFontTexture, to_string(gstate->currentCoins()), 0, 215, 8, 8, 3.0f, HUD_FLOAT_CENTER);
    }
    else {
        this->drawText(whiteFontTexture, "SCORE", 170, 220, 8, 8, 3.0f, HUD_FLOAT_LEFT);
        this->drawText(goldenFontTexture, to_string(gstate->currentCoins()), 285, 220, 8, 8, 3.0f, HUD_FLOAT_LEFT);
        this->drawText(whiteFontTexture, "HIGHSCORE", 85, 260, 8, 8, 3.0f, HUD_FLOAT_LEFT);
        this->drawText(goldenFontTexture, to_string(currentHighScore), 285, 260, 8, 8, 3.0f, HUD_FLOAT_LEFT);
    }
    this->drawText(whiteFontTexture, "PRESS L TO PLAY AGAIN", 100, 570, 8, 8, 2.0f, HUD_FLOAT_LEFT);
}

void HUD::renderPauseScreen(){
    this->drawFadeOverlay(75);
    this->drawText(whiteFontTexture, "PRESS SPACE TO CONTINUE", 100, 250, 8, 8, 2.0f, HUD_FLOAT_LEFT);
}

void HUD::renderInstructions(){
    this->drawFadeOverlay(80);
    this->drawFadeOverlay(80);
    this->drawText(whiteFontTexture, "HOW TO PLAY?", 115, 25, 8, 8, 3.2f, HUD_FLOAT_LEFT);
    this->drawText(whiteFontTexture, "PRESS SPACE TO CONTINUE", 100, 570, 8, 8, 2.0f, HUD_FLOAT_LEFT);
    SDL_Rect box, paraRect;

    box = {20, 60, 460, 500};
    gwin->blit(frameTexture, box);

    int x = 40;
    int y = 90;
    for (int i = 0; i < 2; i++) {
        box = {x, y, 420, 220};
        SDL_SetRenderDrawColor(this->gwin->gRenderer, 222, 159, 71, 255);
        if (i < 1) SDL_RenderDrawLine(this->gwin->gRenderer, box.x, box.y + box.h, box.x + box.w, box.y + box.h);
        this->drawText(
            plainWhiteFontTexture,
            ins[i].name,
            60, y + 15, 16, 16, 1.2f
        );

        paraRect = {60, y + 40, 380, 90};
        this->drawParagraph(ins[i].desc, paraRect, plainBlackFontTexture, 6, 12, 2);
        y += box.h;
    }
}
