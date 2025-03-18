#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "../include/Tetromino.hpp"

class Board {
public:
    Board(int width, int height, int cellSize);
    void draw(sf::RenderWindow &window);
    
    bool isValidPosition(const Tetromino &tetromino, int offsetX, int offsetY) const;
    void placeTetromino(const Tetromino &tetromino);
    
    void clearLines();

private: 
    int mWidth;         // Number of columns
    int mHeight;        // Number of rows
    int mCellSize;      // Pixel size of each cell
    std::vector<std::vector<int>> mGrid; // 2D grid: 0=empty, 1=occupied
};

#endif // !BOARD_HPP
