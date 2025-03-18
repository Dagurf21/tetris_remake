#include "../include/Game.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game()
: mWindow(sf::VideoMode(320, 640), "Tetris SFML"),
    mBoard(10, 20, 32),                 // 10 cols, 20 rows, 32x32 pixels
    mTetromino(TetrominoType::I, 32),   // Start with an I-piece
    mFallTimer(0)
{
    // Set the starting pos for the tetromino 
    mTetromino.setPosition(3, 0);
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
            mBoard.clearLines();

            // spawn new tetromino : TODO: for now always an I-piece
            mTetromino = Tetromino(TetrominoType::I, 32);
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
    mWindow.clear();
    // draw the board 
    mBoard.draw(mWindow);
    // Draw the tetromino
    mTetromino.draw(mWindow);
    mWindow.display();
}
