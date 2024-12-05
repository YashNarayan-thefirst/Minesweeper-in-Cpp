#include "Tile.h"
#include "Toolbox.h"

Tile::Tile(sf::Vector2f _position) : position(_position), state(HIDDEN) {}

// Getters and setters
sf::Vector2f Tile::getLocation() { return position; }
Tile::State Tile::getState() { return state; }
std::array<Tile*, 8>& Tile::getNeighbors() { return neighbors; }
void Tile::setState(State _state) { state = _state; }

void Tile::setNeighbors(std::array<Tile*, 8> n) {
    neighbors = n;
    adj_mine_count = 0;
    auto& tb = ToolBox::getInstance();
    for (const auto* t : neighbors) if (t && tb.board.at((int)t->position.y / 32).at((int)t->position.x / 32)) adj_mine_count++;
}

void Tile::draw() {
    auto& tb = ToolBox::getInstance();
    sf::Sprite tile, modifier;
    tile.setPosition(position.x, position.y);
    modifier.setPosition(position.x, position.y);

    auto drawTile = [&](sf::Texture& t, sf::Texture* m = nullptr) {
        tile.setTexture(t);
        tb.window.draw(tile);
        if (m) {
            modifier.setTexture(*m);
            tb.window.draw(modifier);
        }
    };

    switch (state) {
        case FLAGGED:drawTile(tb.tileHidden, tb.debug_mode && tb.board.at((int)position.y / 32).at((int)position.x / 32) ? &tb.mine : &tb.flag);
            break;
        case HIDDEN:drawTile(tb.tileHidden, tb.debug_mode && tb.board.at((int)position.y / 32).at((int)position.x / 32) ? &tb.mine : nullptr);
            break;
        case REVEALED:
            drawTile(tb.tileRevealed);
            if (adj_mine_count > 0) {
                modifier.setTexture(*tb.nums_list.at(adj_mine_count));
                tb.window.draw(modifier);
            }
            break;
        case EXPLODED:drawTile(tb.tileRevealed, &tb.mine);
            break;
    }
}

// Handles left-click interaction
void Tile::onClickLeft() {
    auto& tb = ToolBox::getInstance();
    const int x = (int)position.x / 32, y = (int)position.y / 32;

    if (state == HIDDEN) {
        if (tb.board[y][x])state = EXPLODED;
        else {
            revealNeighbors();
            state = REVEALED;
        }
    }
}

// Handles right-click interaction
void Tile::onClickRight() {
    switch (state) {
        case HIDDEN: state = FLAGGED; break;
        case FLAGGED: state = HIDDEN; break;
        default: break;
    }
}

// Recursively reveals neighboring tiles
void Tile::revealNeighbors() {
    auto& tb = ToolBox::getInstance();
    const int x = (int)position.x / 32,y = (int)position.y / 32;
    if (state != HIDDEN || tb.board.at(y).at(x)) return;
    state = REVEALED;
    if (!adj_mine_count) {
        for (auto* t : neighbors) {
            if (t) t->revealNeighbors();
        }
    }
}
