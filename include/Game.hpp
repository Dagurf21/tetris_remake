#ifndef GAME_HPP
#define GAME_HPP

#include "../include/Board.hpp"
#include "../include/Tetromino.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

class Game {
public:
    int getScore() const { return mScore; }
    void setScore(int score) { mScore = score; }

    int getLevel() const { return mLevel; }
    void setLevel(int level) { mLevel = level; }

    float getfallDelay() const { return mFallDelay; }
    void setFallDelay(float delay) { mFallDelay = delay; }

    void adjustFallSpeed();

    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow mWindow;
    Board mBoard;           // The tetris board
    Tetromino mTetromino;   // Current Tetromino

    float mFallTimer;
    float mFallDelay;

    // UI Elements
    sf::Font mFont;
    sf::Text mScoreText;
    sf::Text mLevelText;

    int mScore;             // Track the score
    int mLevel;
};

#endif // GAME_HPP
