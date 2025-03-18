#include "../include/Game.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game()
: mWindow(sf::VideoMode(800, 600), "Tetris SFML"),
    mBoard(10, 20, 32),                 // 10 cols, 20 rows, 32x32 pixels
    mTetromino(TetrominoType::I, 32)    // Start with an I-piece
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
            if (event.key.code == sf::Keyboard::Space) {
                mTetromino.rotate();
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    // Update game state
}

void Game::render() {
    mWindow.clear();
    // draw the board 
    mBoard.draw(mWindow);
    // Draw the tetromino
    mTetromino.draw(mWindow);
    mWindow.display();
}
