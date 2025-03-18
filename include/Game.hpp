#ifndef GAME_HPP
#define GAME_HPP

#include "../include/Board.hpp"
#include "../include/Tetromino.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow mWindow;
    Board mBoard;           // The tetris board
    Tetromino mTetromino;   // Current Tetromino
};

#endif // GAME_HPP
