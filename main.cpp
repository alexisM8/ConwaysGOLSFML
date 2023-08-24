#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/VideoMode.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <string>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 1280;
constexpr int ROWS = 64;
constexpr int COLS = 64;
constexpr int SIZE = WINDOW_HEIGHT / ROWS;

struct cell{
    sf::RectangleShape cube;
    std::string state{"Dead"};
};

void init_board(std::array<std::array<cell, ROWS>, COLS>&);
void compute_next_iteration(std::array<std::array<cell, ROWS>, COLS>&);
int count_neighbors(std::array<std::array<cell, ROWS>, COLS>& board, int row, int col);

int main(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Conway's Game Of Life");
    sf::Mouse ms;
    bool held = false;

    std::array<std::array<cell, ROWS>, COLS> board;
    init_board(board);

    while(window.isOpen()){
        sf::Event ev;
        
        while(window.pollEvent(ev)){
            switch(ev.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if(ev.mouseButton.button == sf::Mouse::Left){
                        held = true;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(ev.mouseButton.button == sf::Mouse::Left){
                        held = false;
                    }
                    break;
                case sf::Event::KeyPressed:
                    if(ev.key.code == sf::Keyboard::N){
                        compute_next_iteration(board);
                    }
                    if(ev.key.code == sf::Keyboard::C){
                        for(auto& b: board){
                            for(auto& e: b){
                                e.state = "Dead";
                                e.cube.setFillColor(sf::Color::Black);
                            }
                        }
                    }
                    break;
                default: 
                    break;
            }

        }
        
        if(held){
            sf::Vector2f pos{ms.getPosition(window)};
            int r = pos.x / SIZE;
            int c = pos.y / SIZE;
            board[r][c].cube.setFillColor(sf::Color::Green);
            board[r][c].state = "Alive";
        }

        window.clear(sf::Color(sf::Color::Green));
        for(const auto& b: board){
            for(const auto& c: b){
                window.draw(c.cube);
            }
        }
        window.display();
    }
}

void init_board(std::array<std::array<cell, ROWS>, COLS>& board){
    for(int r = 0; r < ROWS; ++r){
        for(int c = 0; c < COLS; ++c){
            board[r][c].state = "Dead";
            board[r][c].cube.setFillColor(sf::Color::Black);
            board[r][c].cube.setSize({SIZE, SIZE});
            board[r][c].cube.setPosition(r*SIZE,  c*SIZE);
        }
    }
}

int count_neighbors(std::array<std::array<cell, ROWS>, COLS>& board, int row, int col){
    int alive = 0;
    for(int dr = -1; dr <= 1; ++dr){
        for(int dc = -1; dc <= 1; ++dc){
            if(dr != 0 || dc != 0){
                int r = row + dr;
                int c = col + dc;
                if(0 <= r && r < ROWS && 0 <= c && c < COLS){
                    if(board[r][c].state == "Alive"){
                        alive++;
                    }
                }
            }
        }
    }
    return alive;
}

void compute_next_iteration(std::array<std::array<cell, ROWS>, COLS>& board){
    std::array<std::array<cell, ROWS>, COLS> nextBoard{board};

    for(int r = 0; r < ROWS; ++r){
        for(int c = 0; c < COLS; ++c){
            int n = count_neighbors(board, r, c);
            if(board[r][c].state == "Dead"){
                if(n == 3){
                    nextBoard[r][c].state = "Alive";
                    nextBoard[r][c].cube.setFillColor(sf::Color::Green);
                } 
            } else {
                if(n < 2 || n > 3){
                    nextBoard[r][c].state = "Dead";
                    nextBoard[r][c].cube.setFillColor(sf::Color::Black);
                }
            }
        }
    }

    board = nextBoard;
}

