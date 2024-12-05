#pragma once
#include <SFML/Graphics.hpp>
#include "Toolbox.h"

class GameState;

class Tile{
    public:
        enum State{REVEALED, HIDDEN, FLAGGED, EXPLODED};
        Tile(sf::Vector2f _position);
        sf::Vector2f getLocation();
        State getState();
        std::array<Tile*,8>& getNeighbors();
        void setState(State _state);
        void setNeighbors(std::array<Tile*,8> _neighbors);
        void onClickLeft();
        void onClickRight();
        void draw();
        int adj_mine_count;
    protected:
        void revealNeighbors();
    sf::Vector2f position;
    State state = HIDDEN;
    std::array<Tile*,8> neighbors;
    sf::Sprite tile_sprite, flag_sprite, mine_sprite,number_sprite;

};