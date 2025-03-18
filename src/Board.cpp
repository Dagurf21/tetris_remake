#include "../include/Board.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

Board::Board(int width, int height, int cellSize)
: mWidth(width), mHeight(height), mCellSize(cellSize), mGrid(height, std::vector<int>(width, 0))
{
    // Can initialize grid if needed
}

void Board::draw(sf::RenderWindow &window) {
    // Rectrangle shape representing one cell 
    sf::RectangleShape cellShape(sf::Vector2f(mCellSize - 1, mCellSize - 1));

    // loop through the grid and draw each cell.
    for (int row = 0; row < mHeight; row++) {
        for (int col = 0; col < mWidth; col++) {
            cellShape.setPosition(col * mCellSize, row * mCellSize);
            // For now all cells are empty
            cellShape.setFillColor(sf::Color::Black);
            cellShape.setOutlineThickness(1);
            cellShape.setOutlineColor(sf::Color(128, 128, 128));
            window.draw(cellShape);
        }
    }
}
