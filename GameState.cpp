#include "GameState.h"
#include "Toolbox.h"
#include <fstream>
#include <random>
#include <iostream>

// Set neighbors for each tile
void GameState::setNeighbors() {
    for (int j = 0; j < dimensions.y; j++) {
        for (int k = 0; k < dimensions.x; k++) {
            tiles[j][k]->setNeighbors({
            (j > 0 && k > 0) ? tiles[j - 1][k - 1] : nullptr,
            (j > 0) ? tiles[j - 1][k] : nullptr,
            (j > 0 && k + 1 < dimensions.x) ? tiles[j - 1][k + 1] : nullptr,
            (k > 0) ? tiles[j][k - 1] : nullptr,
            (k + 1 < dimensions.x) ? tiles[j][k + 1] : nullptr,
            (j + 1 < dimensions.y && k > 0) ? tiles[j + 1][k - 1] : nullptr,
            (j + 1 < dimensions.y) ? tiles[j + 1][k] : nullptr,
            (j + 1 < dimensions.y && k + 1 < dimensions.x) ? tiles[j + 1][k + 1] : nullptr
        });
        }
    }
}


GameState::GameState(sf::Vector2i _dimensions, int _numberOfMines)
    : dimensions(_dimensions), mine_count(_numberOfMines), status(PLAYING) {
    auto& tb = ToolBox::getInstance();
    tb.board.assign(dimensions.y, std::vector<bool>(dimensions.x, false));

    // Randomly place mines
    for (int i = 0; i < mine_count; i++) {
        while (true) {
            int randRow = rand() % dimensions.y, randCol = rand() % dimensions.x;
            if (!tb.board[randRow][randCol]) {
                tb.board[randRow][randCol] = true;
                break;
            }
        }
    }

    // Initialize tiles
    tiles.resize(dimensions.y, std::vector<Tile*>(dimensions.x, nullptr));
    for (int j = 0; j < dimensions.y; j++) {
        for (int k = 0; k < dimensions.x; k++) {
            tiles[j][k] = new Tile(sf::Vector2f(k * 32, j * 32));
        }
    }
    setNeighbors();
}

// Constructor: initializes from file
GameState::GameState(const char* filepath) : mine_count(0), status(PLAYING) {
    std::ifstream in(filepath);
    if (!in.is_open()) throw std::runtime_error("Failed to open file");
    std::vector<std::vector<char>> rows;
    std::vector<char> tempHolder;
    int prevSize = -1;
    char c;
    // Parse file into rows
    while (in.get(c)) {
        if (c != '\n') {
            tempHolder.push_back(c);
        } else {
            // Skip empty rows
            if (!tempHolder.empty()) {
                if (prevSize == -1) {
                    prevSize = tempHolder.size();
                } else if (prevSize != tempHolder.size()) {
                    throw std::runtime_error("Inconsistent row lengths in board file");
                }
                rows.push_back(tempHolder);
                tempHolder.clear();
            }
        }
    }
    // Add the last row if it's non-empty
    if (!tempHolder.empty()) {
        if (prevSize != tempHolder.size()) throw std::runtime_error("Inconsistent row lengths in board file");
        rows.push_back(tempHolder);
    }
    dimensions = { (int)rows[0].size(), (int)rows.size() };
    if (dimensions.x <= 0 || dimensions.y <= 0) throw std::runtime_error("Invalid board dimensions");
    auto& tb = ToolBox::getInstance();
    tb.board.resize(dimensions.y, std::vector(dimensions.x, false));

    // Populate board
    for (int j = 0; j < dimensions.y; j++) {
        for (int k = 0; k < dimensions.x; k++) {
            tb.board[j][k] = rows[j][k] != '0';
            mine_count += tb.board[j][k];
        }
    }

    tiles.resize(dimensions.y, std::vector<Tile*>(dimensions.x, nullptr));
    for (int j = 0; j < dimensions.y; j++) {
        for (int k = 0; k < dimensions.x; k++) {
            tiles[j][k] = new Tile(sf::Vector2f(k * 32, j * 32));
        }
    }

    setNeighbors();
}


// Getters
GameState::PlayStatus GameState::getPlayStatus() { return status; }
void GameState::setPlayStatus(PlayStatus _status) { status = _status; }
Tile* GameState::getTile(int x, int y) { return tiles[y][x]; }
int GameState::getMineCount() { return mine_count; }

int GameState::getFlagCount() {
    int fc = 0;
    for (auto& row : tiles) {
        for (const auto& tile : row) {
            if (tile->getState() == Tile::FLAGGED) fc++;
        }
    }
    return fc;
}

GameState::~GameState() {
    for (auto& row : tiles) {
        for (const auto& tile : row) {
            delete tile;
        }
    }
}
