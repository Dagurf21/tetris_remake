#include "../include/Game.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <random>

#include <string>

int offsetX = 800;
int offsetY = 220;

Game::Game()
    : mWindow(sf::VideoMode(1920, 1080), "Tetris SFML"),
      mBoard(10, 20, 32, *this),        // 10 cols, 20 rows, 32x32 pixels
      mTetromino(TetrominoType::I, 32), // Start with an I-piece
      mNextTetromino(TetrominoType::T, 32), mFallTimer(0),
      mScore(0), // Initial score is 0
      mLevel(1), mFallDelay(1.0f), mTotalLinesCleared(0),
      mCurrentState(GameState::MENU) {
  if (!bufferClearLine1.loadFromFile("assets/Music/clear_line_1.wav")) {
    std::cerr << "Error loading clear_line_1.wav\n";
  }
  soundClearLine1.setBuffer(bufferClearLine1);

  if (!bufferClearLine2.loadFromFile("assets/Music/clear_line_2.wav")) {
    std::cerr << "Error loading clear_line_2.wav\n";
  }
  soundClearLine2.setBuffer(bufferClearLine2);

  if (!bufferPlaceTetromino1.loadFromFile(
          "assets/Music/place_tetromino_1.wav")) {
    std::cerr << "Error loading place_tetromino_1.wav\n";
  }
  soundPlaceTetromino1.setBuffer(bufferPlaceTetromino1);

  if (!bufferPlaceTetromino2.loadFromFile(
          "assets/Music/place_tetromino_2.wav")) {
    std::cerr << "Error loading place_tetromino_2.wav\n";
  }
  soundPlaceTetromino2.setBuffer(bufferPlaceTetromino2);

  if (!bufferPlaceTetromino3.loadFromFile(
          "assets/Music/place_tetromino_3.wav")) {
    std::cerr << "Error loading place_tetromino_3.wav\n";
  }
  soundPlaceTetromino3.setBuffer(bufferPlaceTetromino3);

  if (!backgroundMusic.openFromFile("assets/Music/background_music.wav")) {
    std::cerr << "Error loading background_music.wav\n";
  }
  backgroundMusic.setLoop(true); // Make it loop indefinitely
  backgroundMusic.play();

  // Set the starting pos for the tetromino
  mTetromino.setPosition(3, 0);

  // Load a font
  if (!mFont.loadFromFile("assets/0xProtoNerdFont-Bold.ttf")) {
    std::cerr << "Error loading font!" << "\n";
  }
  // score text
  mScoreText.setFont(mFont);
  mScoreText.setString("Score: 0");
  mScoreText.setCharacterSize(24);
  mScoreText.setFillColor(sf::Color::White);
  mScoreText.setPosition(offsetX + 340, offsetY + 20);

  // Set up the level text
  mLevelText.setFont(mFont);
  mLevelText.setString("Level: 1");
  mLevelText.setCharacterSize(24);
  mLevelText.setFillColor(sf::Color::White);
  mLevelText.setPosition(offsetX + 340, offsetY + 50);

  mMenu = new Menu(mFont, mWindow.getSize());

  mHoldUsed = false;
}

void Game::run() {
  sf::Clock clock;
  while (mWindow.isOpen()) {
    sf::Time deltaTime = clock.restart();

    if (mCurrentState == GameState::MENU) {
      processMenuEvents();
      renderMenu();
    } else if (mCurrentState == GameState::PLAYING) {
      processEvents();
      update(deltaTime);
      render();
    } else if (mCurrentState == GameState::GAMEOVER) {
      processGameOverEvents();
      renderGameOver();
    }
  }
}

TetrominoType getRandomTetrominoType() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  // There are 8 tetromino types
  std::uniform_int_distribution<> distrib(0, 6);
  int randomIndex = distrib(gen);
  return static_cast<TetrominoType>(randomIndex);
}

void Game::processEvents() {
  sf::Event event;
  while (mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mWindow.close();
    }

    // Keyboard input logic
    if (event.type == sf::Event::KeyPressed) {

      // Rotate with up arrow
      if (event.key.code == sf::Keyboard::Up) {

        // Attempt to rotate the tetromino
        mTetromino.rotate();

        // If the new rotated position is invalid, try wall kick offsets
        if (!mBoard.isValidPosition(mTetromino, 0, 0)) {
          // Define candidate offsets (you can tweak these values)
          std::vector<sf::Vector2i> wallKickOffsets = {
              sf::Vector2i(-1, 0), // kick left
              sf::Vector2i(1, 0),  // kick right
              sf::Vector2i(0, -1)  // kick up
          };

          bool kicked = false;
          for (const auto &offset : wallKickOffsets) {
            if (mBoard.isValidPosition(mTetromino, offset.x, offset.y)) {
              // Apply the valid offset
              sf::Vector2i pos = mTetromino.getPosition();
              mTetromino.setPosition(pos.x + offset.x, pos.y + offset.y);
              kicked = true;
              break;
            }
          }

          // If no wall kick offset worked, revert the rotation
          if (!kicked) {
            // Since our rotation rotates 90 degrees clockwise,
            // rotating three more times will return to the original
            // orientation.
            mTetromino.rotate();
            mTetromino.rotate();
            mTetromino.rotate();
          }
        }
      }

      // Move left
      else if (event.key.code == sf::Keyboard::Left) {
        if (mBoard.isValidPosition(mTetromino, -1, 0)) {
          auto pos = mTetromino.getPosition();
          mTetromino.setPosition(pos.x - 1, pos.y);
        }
      }

      // Move right
      else if (event.key.code == sf::Keyboard::Right) {
        if (mBoard.isValidPosition(mTetromino, 1, 0)) {
          auto pos = mTetromino.getPosition();
          mTetromino.setPosition(pos.x + 1, pos.y);
        }
      }

      // Move down (soft drop)
      else if (event.key.code == sf::Keyboard::Down) {
        if (mBoard.isValidPosition(mTetromino, 0, 1)) {
          auto pos = mTetromino.getPosition();
          mTetromino.setPosition(pos.x, pos.y + 1);
        }
      }

      // Hard Drop using Space Bar
      else if (event.key.code == sf::Keyboard::Space) {
        // loop until tetromino cannot move any more down
        while (mBoard.isValidPosition(mTetromino, 0, 1)) {
          auto pos = mTetromino.getPosition();
          mTetromino.setPosition(pos.x, pos.y + 1);
        }
        // Lock the tetromino into board
        mBoard.placeTetromino(mTetromino);
        // Clear completed lines if any
        int cleared = mBoard.clearLines();

        if (cleared > 0) {
          int calculatedScore = calculateScore(cleared);
          mScore += calculatedScore;
          mTotalLinesCleared += cleared;
          mLevel = (mTotalLinesCleared / 10) + 1;
          mScoreText.setString("Score: " + std::to_string(mScore));
          mLevelText.setString("Level: " + std::to_string(mLevel));
          adjustFallSpeed();
        }

        // Spawn new tetromino (TODO: now always I-piece);
        mTetromino = mNextTetromino;
        mTetromino.setPosition(3, 0);
        mNextTetromino = Tetromino(getRandomTetrominoType(), 32);
        mHoldUsed = false; // Reset hold usage for the new drop
        if (!mBoard.isValidPosition(mTetromino, 0, 0)) {
          mCurrentState = GameState::GAMEOVER;
        }
      }

      // Hold / Swap Piece using the C key
      else if (event.key.code == sf::Keyboard::C) {
        if (!mHoldUsed) {
          if (!mHeldTetromino.has_value()) {
            // Hold current piece and spawn next piece as current
            mHeldTetromino = mTetromino;
            mTetromino = mNextTetromino;
            mTetromino.setPosition(3, 0);
            mNextTetromino = Tetromino(getRandomTetrominoType(), 32);
          } else {
            // Swap held piece with the current piece
            std::swap(mTetromino, mHeldTetromino.value());
            mTetromino.setPosition(3, 0);
          }
          mHoldUsed = true;
        }
      }
    }
  }
}

void Game::update(sf::Time deltaTime) {
  // Increase fall timer based on elapsed time
  mFallTimer += deltaTime.asSeconds();
  const float FALL_DELAY = 0.5f; // tetromino falls every 0.5 seconds

  if (mFallTimer >= FALL_DELAY) {
    mFallTimer = 0.f; // Reset the timer

    // Check if tetromino can move down
    if (mBoard.isValidPosition(mTetromino, 0, 1)) {
      sf::Vector2i pos = mTetromino.getPosition();
      mTetromino.setPosition(pos.x, pos.y + 1);
    } else {
      // cant move down: lock tetromino into board
      mBoard.placeTetromino(mTetromino);

      // clear any completed lines
      int cleared = mBoard.clearLines();
      if (cleared > 0) {
        int calculatedScore = calculateScore(cleared);
        mScore += calculatedScore;

        mTotalLinesCleared += cleared;

        mLevel = (mTotalLinesCleared / 10) + 1;

        mLevelText.setString("Level: " + std::to_string(mLevel));
        mScoreText.setString("Score: " + std::to_string(mScore));
        adjustFallSpeed();
      }

      // spawn new tetromino : TODO: for now always an I-piece
      TetrominoType randomType = getRandomTetrominoType();
      mTetromino = Tetromino(randomType, 32);
      mTetromino.setPosition(3, 0);

      // Check for GameOver: if the new piece is invalid right away
      if (!mBoard.isValidPosition(mTetromino, 0, 0)) {

        // In Game::update() or in the event branch that spawns a new tetromino:
        if (!mBoard.isValidPosition(mTetromino, 0, 0)) {
          mCurrentState = GameState::GAMEOVER;
          // Optionally: you might want to stop further updates or lock input.
        }
      }
    }
    mFallTimer = 0;
  }
}

void Game::render() {
  mWindow.clear(sf::Color(30, 30, 30));

  // --- Draw main board area ---
  sf::RectangleShape boardBackground(sf::Vector2f(320, 640));
  boardBackground.setFillColor(sf::Color(50, 50, 50));
  boardBackground.setPosition(offsetX, offsetY);
  mWindow.draw(boardBackground);

  sf::RectangleShape boardBorder(sf::Vector2f(320, 640));
  boardBorder.setFillColor(sf::Color::Transparent);
  boardBorder.setOutlineThickness(3);
  boardBorder.setOutlineColor(sf::Color::White);
  boardBorder.setPosition(offsetX, offsetY);
  mWindow.draw(boardBorder);

  // --- Draw board contents (grid, ghost, current tetromino) ---
  mBoard.draw(mWindow, offsetX, offsetY);

  // Draw ghost piece
  Tetromino ghost = mTetromino;
  while (mBoard.isValidPosition(ghost, 0, 1)) {
    auto pos = ghost.getPosition();
    ghost.setPosition(pos.x, pos.y + 1);
  }
  ghost.drawGhost(mWindow, offsetX, offsetY);

  // Draw current tetromino
  mTetromino.draw(mWindow, offsetX, offsetY);

  // --- UI: Score and Level ---
  mWindow.draw(mScoreText);
  mWindow.draw(mLevelText);

  // ------------------------------------------------------
  // Next box on the right (below Score/Level)
  // ------------------------------------------------------
  int nextBoxWidth = 150;
  int nextBoxHeight = 150;
  int nextBoxX = offsetX + 340;
  int nextBoxY = offsetY + 90; // below the score/level

  sf::RectangleShape nextBox(sf::Vector2f(nextBoxWidth, nextBoxHeight));
  nextBox.setFillColor(sf::Color(50, 50, 50));
  nextBox.setOutlineColor(sf::Color::White);
  nextBox.setOutlineThickness(2);
  nextBox.setPosition(nextBoxX, nextBoxY);
  mWindow.draw(nextBox);

  sf::Text nextLabel;
  nextLabel.setFont(mFont);
  nextLabel.setString("Next");
  nextLabel.setCharacterSize(24);
  nextLabel.setFillColor(sf::Color::White);
  nextLabel.setPosition(nextBoxX, nextBoxY - 30);
  mWindow.draw(nextLabel);

  // Center a 4×4 piece in a 150×150 box:
  {
    int offsetCenterX = nextBoxX + (nextBoxWidth - 128) / 2;
    int offsetCenterY = nextBoxY + (nextBoxHeight - 128) / 2;
    mNextTetromino.draw(mWindow, offsetCenterX, offsetCenterY);
  }

  // ------------------------------------------------------
  // Hold box on the left, moved further down
  // ------------------------------------------------------
  int holdBoxWidth = 150;
  int holdBoxHeight = 150;
  // Move it "a bit more down": e.g. 300 pixels below offsetY
  int holdBoxX = offsetX - (holdBoxWidth + 20);
  int holdBoxY = offsetY + 300;

  sf::RectangleShape holdBox(sf::Vector2f(holdBoxWidth, holdBoxHeight));
  holdBox.setFillColor(sf::Color(50, 50, 50));
  holdBox.setOutlineColor(sf::Color::White);
  holdBox.setOutlineThickness(2);
  holdBox.setPosition(holdBoxX, holdBoxY);
  mWindow.draw(holdBox);

  sf::Text holdLabel;
  holdLabel.setFont(mFont);
  holdLabel.setString("Hold");
  holdLabel.setCharacterSize(24);
  holdLabel.setFillColor(sf::Color::White);
  holdLabel.setPosition(holdBoxX, holdBoxY - 30);
  mWindow.draw(holdLabel);

  // Draw the held tetromino if it exists
  if (mHeldTetromino.has_value()) {
    // Temporarily reset its position to (0,0) so it doesn't get drawn
    // off-screen
    sf::Vector2i oldPos = mHeldTetromino->getPosition();
    mHeldTetromino->setPosition(0, 0);

    int offsetCenterX = holdBoxX + (holdBoxWidth - 128) / 2;
    int offsetCenterY = holdBoxY + (holdBoxHeight - 128) / 2;
    mHeldTetromino->draw(mWindow, offsetCenterX, offsetCenterY);

    // Restore the original position after drawing
    mHeldTetromino->setPosition(oldPos.x, oldPos.y);
  }

  mWindow.display();
}

int Game::calculateScore(int linesCleared) {
  switch (linesCleared) {
  case 1:
    return 40 * (mLevel + 1);
    break;
  case 2:
    return 100 * (mLevel + 1);
    break;
  case 3:
    return 300 * (mLevel + 1);
    break;
  case 4:
    return 1200 * (mLevel + 1);
    break;
  default: // Should never reach here
    return 0;
    break;
  }
}

void Game::adjustFallSpeed() {
  mFallDelay = std::max(0.1f, 1.0f - (mLevel * 0.1f));
}

void Game::processMenuEvents() {
  sf::Event event;
  while (mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mWindow.close();
    }
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Up) {
        mMenu->moveUp();
      } else if (event.key.code == sf::Keyboard::Down) {
        mMenu->moveDown();
      } else if (event.key.code == sf::Keyboard::Enter) {
        // Based on selection, change state
        if (mMenu->getSelectedIndex() == 0) {
          mCurrentState = GameState::PLAYING;
        } else if (mMenu->getSelectedIndex() == 1) {
          mWindow.close();
        }
      }
    }
  }
}

void Game::renderMenu() {
  mWindow.clear(sf::Color(30, 30, 30));
  mMenu->render(mWindow);
  mWindow.display();
}

void Game::processGameOverEvents() {
  sf::Event event;
  while (mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      mWindow.close();
    if (event.type == sf::Event::KeyPressed) {
      // For example, press Enter to return to the main menu.
      if (event.key.code == sf::Keyboard::Enter) {
        resetGame(); // Optionally reset game variables for a new game.
        mCurrentState = GameState::MENU;
      }
    }
  }
}

void Game::renderGameOver() {
  mWindow.clear(sf::Color::Black);

  sf::Text gameOverText;
  gameOverText.setFont(mFont);
  gameOverText.setString("Game Over");
  gameOverText.setCharacterSize(64);
  gameOverText.setFillColor(sf::Color::Red);
  // Center the text
  sf::FloatRect goBounds = gameOverText.getLocalBounds();
  gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f,
                         goBounds.top + goBounds.height / 2.0f);
  gameOverText.setPosition(mWindow.getSize().x / 2.0f,
                           mWindow.getSize().y / 2.0f - 50);

  sf::Text promptText;
  promptText.setFont(mFont);
  promptText.setString("Press Enter to return to Main Menu");
  promptText.setCharacterSize(24);
  promptText.setFillColor(sf::Color::White);
  sf::FloatRect ptBounds = promptText.getLocalBounds();
  promptText.setOrigin(ptBounds.left + ptBounds.width / 2.0f,
                       ptBounds.top + ptBounds.height / 2.0f);
  promptText.setPosition(mWindow.getSize().x / 2.0f,
                         mWindow.getSize().y / 2.0f + 20);

  mWindow.draw(gameOverText);
  mWindow.draw(promptText);
  mWindow.display();
}

void Game::resetGame() {
  // Reset game variables to their initial state
  mScore = 0;
  mLevel = 1;
  mTotalLinesCleared = 0;

  mScoreText.setString("Score: 0");
  mLevelText.setString("Level: 1");

  mBoard.reset();

  // Optionally, reset the board and tetromino state.
  // For example, reinitialize the tetromino to start position.
  mTetromino = Tetromino(TetrominoType::I, 32);
  mTetromino.setPosition(3, 0);

  mFallTimer = 0;
}
