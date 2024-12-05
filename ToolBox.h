#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"
#include <vector>

class GameState;

class ToolBox {
    public:
        std::vector<std::vector<bool>> board;
        sf::RenderWindow window;
        GameState* gameState;
        Button* debugButton;
        Button* newGameButton;
        Button* testButton1;
        Button* testButton2;
        // Declare and load textures
        sf::Texture tileHidden, tileRevealed, flag, mine;
        sf::Texture one, two, three, four, five, six, seven, eight;
        sf::Texture happy, win, lose, digits, debug, test1, test2;
        std::vector<sf::Texture*> nums_list = {nullptr, &one, &two, &three, &four, &five, &six, &seven, &eight};


        bool debug_mode = false;
        static ToolBox& getInstance() {
            static ToolBox instance;
            return instance;
        }
        int gameLoop();
        ~ToolBox();
        int mine_count = 0;
    private:
        ToolBox();
};

