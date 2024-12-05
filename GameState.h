#pragma once
#include "Tile.h"
#include <vector>

class Tile;

class GameState {
    public:
        enum PlayStatus { WIN, LOSS, PLAYING };

        GameState(sf::Vector2i _dimensions = sf::Vector2i(25,16), int _numberOfMines = 50);
        GameState(const char* filepath);

        int getFlagCount();
        int getMineCount();
        Tile* getTile(int x, int y);
        PlayStatus getPlayStatus();
        void setPlayStatus(PlayStatus _status);
        ~GameState();
        void setNeighbors();
        int mine_count;
    private:
        sf::Vector2i dimensions;
        PlayStatus status;
        std::vector<std::vector<Tile*>> tiles;
};
