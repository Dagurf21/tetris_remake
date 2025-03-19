#include "../include/Tetromino.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <cstdlib>
#include <random>

Tetromino::Tetromino(TetrominoType type, int cellSize)
: mType(type), mCellSize(cellSize), mPosition(0, 0)
{
    intializeShape();
}

void Tetromino::intializeShape() {
    // Initialize mShape based on the Tetromino type
    if (mType == TetrominoType::I){
        // I-Piece: 4x4 representation
        mShape = {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        };
    }
    else if (mType == TetrominoType::O) {
        // O-Piece: 2x2 representation
        mShape = {
            {1,1},
            {1,1}
        };
    }
    else if (mType == TetrominoType::T) {
        // T-Piece: 3x3 representation
        mShape = {
            {0,1,0},
            {1,1,1},
            {0,0,0}
        };
    }
    else if (mType == TetrominoType::Z) {
        // Z-Piece: 3x3 representation
        mShape = {
            {1,1,0},
            {0,1,1},
            {0,0,0}
        };
    }
    else if (mType == TetrominoType::S) {
        // S-Piece: 3x3 representation
        mShape = {
            {0,1,1},
            {1,1,0},
            {0,0,0}
        };
    }
    else if (mType == TetrominoType::L) {
        // L-Piece: 3x3 representation
        mShape = {
            {0,1,0},
            {0,1,0},
            {0,1,1}
        };
    }
    else if (mType == TetrominoType::J) {
        // J-Piece: 3x3 representation
        mShape = {
            {0,1,0},
            {0,1,0},
            {1,1,0}
        };
    }
}


void Tetromino::draw(sf::RenderWindow &window) {
    // Draw each filled cell of the tetromino 
    for(size_t row = 0; row < mShape.size(); ++row){
        for (size_t col = 0; col < mShape[row].size(); ++col) {
            if(mShape[row][col]) {
                sf::RectangleShape cell(sf::Vector2f(mCellSize - 1, mCellSize - 1));
                // Choose color based on type
                cell.setFillColor(mType == TetrominoType::I ? sf::Color::Cyan : sf::Color::White);
                cell.setPosition((mPosition.x + col) * mCellSize, (mPosition.y + row) * mCellSize);
                window.draw(cell);
            }
        }
    }
}

void Tetromino::rotate() {
    // Rotate the tetromino shapte clockwise
    size_t rows = mShape.size();
    size_t cols = mShape[0].size();
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows, 0));

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            rotated[col][rows - 1 - row] = mShape[row][col];
        }
    }
    mShape = rotated;
}

void Tetromino::setPosition(int x, int y) {
    mPosition.x = x;
    mPosition.y = y;
}

sf::Vector2i Tetromino::getPosition() const {
    return mPosition;
}

const std::vector<std::vector<int>>& Tetromino::getShape() const {
    return mShape;
}
