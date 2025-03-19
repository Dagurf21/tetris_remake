#include "../include/Board.hpp"
#include "../include/Game.hpp"
#include "../include/Tetromino.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

Board::Board(int width, int height, int cellSize, Game& game)
    : mGame(game), 
    mWidth(width),
    mHeight(height), 
    mCellSize(cellSize), 
    mGrid(height, 
    std::vector<int>(width, 0))
{
    // Can initialize grid if needed
}

void Board::draw(sf::RenderWindow &window, int offsetX, int offsetY) {
    // Rectrangle shape representing one cell 
    sf::RectangleShape cellShape(sf::Vector2f(mCellSize - 1, mCellSize - 1));

    // loop through the grid and draw each cell.
    for (int row = 0; row < mHeight; row++) {
        for (int col = 0; col < mWidth; col++) {
            cellShape.setPosition(
                    offsetX + col * mCellSize,
                    offsetY + row * mCellSize
            );

            if (mGrid[row][col] == 0) {
                cellShape.setFillColor(sf::Color::Black); 
            } else {
                cellShape.setFillColor(sf::Color::White); // or another visible color
            }
            cellShape.setOutlineThickness(1);
            cellShape.setOutlineColor(sf::Color(128, 128, 128));
            window.draw(cellShape);
        }
    }
}

bool Board::isValidPosition(const Tetromino &tetromino, int offsetX, int offsetY) const {
    const auto &shape = tetromino.getShape();
    sf::Vector2i pos = tetromino.getPosition();

    // Check each cell of the tetromino
    for (size_t row = 0; row < shape.size(); row++) {
        for (size_t col = 0; col < shape[row].size(); col++) {
            if (shape[row][col]) {
                int newX = pos.x + col + offsetX;
                int newY = pos.y + row + offsetY;

                // Check if tetromino is outside boundries
                if (newX < 0 || newX >= mWidth || newY < 0 || newY >= mHeight)
                    return false;

                if (mGrid[newY][newX] != 0)
                    return false;
            }
        }
    }
    return true;
}

void Board::placeTetromino(const Tetromino &tetromino) {
    const auto &shape = tetromino.getShape();
    sf::Vector2i pos = tetromino.getPosition();
    for (size_t row = 0; row < shape.size(); row++) {
        for (size_t col = 0; col < shape[row].size(); col++){
            if (shape[row][col]) {
                int gridX = pos.x + col;
                int gridY = pos.y + row;
                if (gridX >= 0 && gridX < mWidth && gridY >= 0 && gridY < mHeight) {
                    mGrid[gridY][gridX] = 1; // Mark the cell as occupied
                }
            }
        }
    }
}

int Board::clearLines() {
    int linesCleared = 0;
    for (int row = 0; row < mHeight; row++) {
        bool isFull = true;
        for (int col = 0; col < mWidth; col++) {
            if (mGrid[row][col] == 0){
                isFull = false;
                break;
            }
        }
        // If the row is full, shift everything above it down
        if (isFull) {
            // Shift the rows down by one
            for (int r = row; r > 0; r--) {
                mGrid[r] = mGrid[r - 1];
            }

            mGrid[0] = std::vector<int>(mWidth, 0);
            row--;
            linesCleared++;
        }
    }

    // Increase level after clearing 10 lines 
    if (linesCleared > 0) {
        mGame.setScore(mGame.getScore() + 100 * linesCleared);
        mGame.setLevel(mGame.getLevel() + linesCleared / 10);
        mGame.adjustFallSpeed();
    }
    return linesCleared;
}
