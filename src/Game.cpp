#include "../include/Game.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <random>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>


#include <filesystem>
#include <string>

int offsetX = 800;
int offsetY = 220;

Game::Game()
: mWindow(sf::VideoMode(1920, 1080), "Tetris SFML"),
    mBoard(10, 20, 32, *this),          // 10 cols, 20 rows, 32x32 pixels
    mTetromino(TetrominoType::I, 32),   // Start with an I-piece
    mFallTimer(0),
    mScore(0),                          // Initial score is 0
    mLevel(1),
    mFallDelay(1.0f)
{
    // Set the starting pos for the tetromino 
    mTetromino.setPosition(3, 0);

    // Load a font
    if (!mFont.loadFromFile("assets/0xProtoNerdFont-Bold.ttf")){
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
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
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
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        
        //Keyboard input logic
        if (event.type == sf::Event::KeyPressed) {
            
            // Rotate with up arrow
            if (event.key.code == sf::Keyboard::Up) {
            
                // Attempt to rotate the tetromino
                mTetromino.rotate();
        
                // If the new rotated position is invalid, try wall kick offsets
                if (!mBoard.isValidPosition(mTetromino, 0, 0)) {
                    // Define candidate offsets (you can tweak these values)
                    std::vector<sf::Vector2i> wallKickOffsets = {
                        sf::Vector2i(-1, 0),  // kick left
                        sf::Vector2i(1, 0),   // kick right
                        sf::Vector2i(0, -1)   // kick up
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
                        // rotating three more times will return to the original orientation.
                        mTetromino.rotate();
                        mTetromino.rotate();
                        mTetromino.rotate();
                    }
                }
            }
       
            // Move left
            else if (event.key.code == sf::Keyboard::Left) {
                if (mBoard.isValidPosition(mTetromino, -1, 0)){
                    auto pos = mTetromino.getPosition();
                    mTetromino.setPosition(pos.x - 1, pos.y);
                }
            }

            // Move right
            else if (event.key.code == sf::Keyboard::Right) {
                if (mBoard.isValidPosition(mTetromino, 1, 0)){
                    auto pos = mTetromino.getPosition();
                    mTetromino.setPosition(pos.x + 1, pos.y);
                }
            }
            
            // Move down (soft drop)
            else if (event.key.code == sf::Keyboard::Down) {
                if (mBoard.isValidPosition(mTetromino, 0, 1)){
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
                    mScore += 100 * cleared;
                    mScoreText.setString("Score: " + std::to_string(mScore));
                }

                // Spawn new tetromino (TODO: now always I-piece);
                TetrominoType randomType = getRandomTetrominoType();
                mTetromino = Tetromino(randomType, 32);
                mTetromino.setPosition(3, 0);

                // Check for GameOver: if the new piece is invalid right away 
                if (!mBoard.isValidPosition(mTetromino, 0, 0)){
                    // TODO: Game over menu
                }
            }
            mFallTimer = 0;
        }
    }
}

void Game::update(sf::Time deltaTime) {
    // Increase fall timer based on elapsed time 
    mFallTimer += deltaTime.asSeconds();
    const float FALL_DELAY = 0.5f;      // tetromino falls every 0.5 seconds 

    if (mFallTimer >= FALL_DELAY) {
        mFallTimer = 0.f;   // Reset the timer

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
                // TODO: Implement level inceremtns and update the score
                std::cout << "Line cleared " << cleared << ", new score: " << mScore << std::endl;
                mScore += 100 * cleared;
                mScoreText.setString("Score: " + std::to_string(mScore));
            }

            // spawn new tetromino : TODO: for now always an I-piece
            TetrominoType randomType = getRandomTetrominoType();
            mTetromino = Tetromino(randomType, 32);
            mTetromino.setPosition(3, 0);

            // Check for GameOver: if the new piece is invalid right away 
            if (!mBoard.isValidPosition(mTetromino, 0, 0)){
                // TODO: Game over menu
            }
        }
        mFallTimer = 0;
    }
}

void Game::render() {
    mWindow.clear(sf::Color(30, 30, 30));
    // draw the board area background
    sf::RectangleShape boardBackground(sf::Vector2f(320, 640)); // Board is 10*32 by 20*32 pixels
    boardBackground.setFillColor(sf::Color(50, 50, 50));
    boardBackground.setPosition(offsetX, offsetY);
    mWindow.draw(boardBackground);

    // Draw a white border around the board
    sf::RectangleShape boardBorder(sf::Vector2f(320, 640));
    boardBorder.setFillColor(sf::Color::Transparent);
    boardBorder.setOutlineThickness(3);
    boardBorder.setOutlineColor(sf::Color::White);
    boardBorder.setPosition(offsetX, offsetY);
    mWindow.draw(boardBorder);
    
    // Draw the game board 
    mBoard.draw(mWindow, offsetX, offsetY);

    // Draw the tetromino
    mTetromino.draw(mWindow, offsetX, offsetY);
    
    // Draw the UI sidebar (just the score text)
    mWindow.draw(mScoreText);

    // Display everything
    mWindow.display();
}

void Game::adjustFallSpeed() {
    mFallDelay = std::max(0.1f, 1.0f - (mLevel * 0.1f));
}
