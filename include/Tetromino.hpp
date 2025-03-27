#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

enum class TetrominoType { I, O, T, S, Z, J, L };

class Tetromino {
public:
  Tetromino(TetrominoType type, int cellSize);

  // Draw the tetromino on the given window
  void draw(sf::RenderWindow &window, int offsetX, int offsetY);

  // Draw the ghost piece
  void drawGhost(sf::RenderWindow &window, int offsetX, int offsetY);

  // Rotate the tetromino clockwise
  void rotate();

  // set the position on the board in grid coords
  void setPosition(int x, int y);

  TetrominoType getType() const { return mType; }

  // Get the current grid position
  sf::Vector2i getPosition() const;

  // Return the current shape as a 2D vector
  const std::vector<std::vector<int>> &getShape() const;

private:
  TetrominoType mType;
  int mCellSize;
  sf::Vector2i mPosition; // grid coords
  std::vector<std::vector<int>>
      mShape; // 2D array representing the tetromino (1=filled, 0=empty)

  // Helper function
  void intializeShape();
};

#endif // TETROMINO_HPP
