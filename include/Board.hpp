#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "../include/Tetromino.hpp"

class Game;

class Board {
public:
    Board(int width, int height, int cellSize, Game& game);
        // : mGame(game), mWidth(width), mHeight(height), mCellSize(cellSize), mGrid(heigt, std::vector<int>(width, 0)) {}

    void draw(sf::RenderWindow &window, int offsetX = 0, int offsetY = 0);
    bool isValidPosition(const Tetromino &tetromino, int offsetX, int offsetY) const;
    void placeTetromino(const Tetromino &tetromino);
    void adjustFallSpeed(); 
    int clearLines();
    void reset();

private: 
    Game& mGame;
    int mWidth;         // Number of columns
    int mHeight;        // Number of rows
    int mCellSize;      // Pixel size of each cell
    std::vector<std::vector<int>> mGrid; // 2D grid: 0=empty, 1=occupied
};

#endif // !BOARD_HPP
