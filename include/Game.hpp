#ifndef GAME_HPP
#define GAME_HPP

#include "../include/Board.hpp"
#include "../include/GameState.hpp"
#include "../include/Menu.hpp"
#include "../include/Tetromino.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <optional>

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
  int calculateScore(int linesCleared);

  GameState mCurrentState;
  Menu *mMenu;

  Tetromino mNextTetromino;
  std::optional<Tetromino> mHeldTetromino;
  bool mHoldUsed;

  void processMenuEvents();
  void renderMenu();

  void processGameOverEvents();
  void renderGameOver();

  void resetGame();

  sf::RenderWindow mWindow;
  Board mBoard;         // The tetris board
  Tetromino mTetromino; // Current Tetromino

  float mFallTimer;
  float mFallDelay;

  // UI Elements
  sf::Font mFont;
  sf::Text mScoreText;
  sf::Text mLevelText;

  int mScore; // Track the score
  int mLevel;
  int mTotalLinesCleared;

  sf::SoundBuffer bufferClearLine1;
  sf::SoundBuffer bufferClearLine2;
  sf::SoundBuffer bufferPlaceTetromino1;
  sf::SoundBuffer bufferPlaceTetromino2;
  sf::SoundBuffer bufferPlaceTetromino3;

  sf::Sound soundClearLine1;
  sf::Sound soundClearLine2;
  sf::Sound soundPlaceTetromino1;
  sf::Sound soundPlaceTetromino2;
  sf::Sound soundPlaceTetromino3;

  sf::Music backgroundMusic;
};

#endif // GAME_HPP
