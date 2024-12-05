#include "ToolBox.h"
#include "minesweeper.h"


//Helpers
// Handle right mouse click actions
void handleRightClick(int xTileCoord, int yTileCoord) {
    auto& tb = ToolBox::getInstance();
    if (xTileCoord < tb.board[0].size() && yTileCoord < tb.board.size() && tb.gameState->getPlayStatus() == GameState::PLAYING) tb.gameState->getTile(xTileCoord, yTileCoord)->onClickRight();
}


// Check if any mines have exploded
bool checkExplodedMines() {
    const auto& tb = ToolBox::getInstance();
    for (int j = 0; j < tb.board.size(); j++) {
        for (int k = 0; k < tb.board[0].size(); k++) {
            if(tb.gameState->getTile(k, j)->getState() == Tile::EXPLODED) return true;
        }
    }
    return false;
}

// Reveal all mines on the board
void revealAllMines() {
    const auto& tb = ToolBox::getInstance();
    for (int j = 0; j < tb.board.size(); j++) {
        for (int k = 0; k < tb.board[0].size(); k++) {
            if (tb.board[j][k]) {
                tb.gameState->getTile(k, j)->setState(Tile::EXPLODED);
            }
        }
    }
}

// Check win condition
bool checkWinCondition() {
    const auto& tb = ToolBox::getInstance();
    for (int j = 0; j < tb.board.size(); j++) {
        for (int k = 0; k < tb.board[0].size(); k++) {
            if (!tb.board[j][k] && tb.gameState->getTile(k, j)->getState() != Tile::REVEALED) {
                return false;
            }
        }
    }
    return true;
}

// Utility function: Check if a position is within specified bounds
bool checkBounds(const sf::Vector2i& pos, int xMin, int xMax, int yMin, int yMax) {return pos.x >= xMin && pos.x <= xMax && pos.y >= yMin && pos.y <= yMax;}

void handleLeftClick(const sf::Vector2i& mousePosition, int xTileCoord, int yTileCoord) {
    auto& tb = ToolBox::getInstance();
    // Check button click regions
    if (checkBounds(mousePosition, 496, 560, 510, 574)) tb.debugButton->onClick();
    else if (checkBounds(mousePosition, 368, 432, 510, 574)) tb.newGameButton->onClick();
    else if (checkBounds(mousePosition, 560, 624, 510, 574)) tb.testButton1->onClick();
    else if (checkBounds(mousePosition, 624, 688, 510, 574)) tb.testButton2->onClick();

    // Handle tile clicks
    else if (xTileCoord < tb.board[0].size() && yTileCoord < tb.board.size() && tb.gameState->getPlayStatus() == GameState::PLAYING) tb.gameState->getTile(xTileCoord, yTileCoord)->onClickLeft();

    // Check for exploded mines and update play status
    if (checkExplodedMines()) {
        tb.gameState->setPlayStatus(GameState::PlayStatus::LOSS);
        revealAllMines();
    }

    // Check for a win
    if (checkWinCondition()) tb.gameState->setPlayStatus(GameState::PlayStatus::WIN);
}

ToolBox::ToolBox() {
    // Initialize the game window
    window.create(sf::VideoMode(800, 600), "P4 - Minesweeper, <Yash Narayan>");
    debug_mode = false;
    gameState = nullptr;
    debugButton = nullptr;
    newGameButton = nullptr;
    testButton1 = nullptr;
    testButton2 = nullptr;

    // Load textures
    tileHidden.loadFromFile("../images/tile_hidden.png");
    tileRevealed.loadFromFile("../images/tile_revealed.png");

    flag.loadFromFile("../images/flag.png");
    mine.loadFromFile("../images/mine.png");

    one.loadFromFile("../images/number_1.png");
    two.loadFromFile("../images/number_2.png");
    three.loadFromFile("../images/number_3.png");
    four.loadFromFile("../images/number_4.png");
    five.loadFromFile("../images/number_5.png");
    six.loadFromFile("../images/number_6.png");
    seven.loadFromFile("../images/number_7.png");
    eight.loadFromFile("../images/number_8.png");

    happy.loadFromFile("../images/face_happy.png");
    win.loadFromFile("../images/face_win.png");
    lose.loadFromFile("../images/face_lose.png");

    digits.loadFromFile("../images/digits.png");
    debug.loadFromFile("../images/debug.png");

    test1.loadFromFile("../images/test_1.png");
    test2.loadFromFile("../images/test_2.png");

}

// Define the main game loop
int ToolBox::gameLoop() {
    auto& tb = getInstance();

    while (tb.window.isOpen()) {
        sf::Event event;

        // Handle events
        while (tb.window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    tb.window.close();
                    break;

                case sf::Event::MouseButtonPressed: {
                    auto mousePosition = sf::Mouse::getPosition(tb.window);
                    const int xTileCoord = mousePosition.x / 32,yTileCoord = mousePosition.y / 32;
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left:
                            handleLeftClick(mousePosition, xTileCoord, yTileCoord);
                            break;
                        case sf::Mouse::Right:
                            handleRightClick(xTileCoord, yTileCoord);
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        tb.window.clear(sf::Color::White);
        render();
        tb.window.display();
    }

    return 0;
}


ToolBox::~ToolBox() {
    delete gameState;
    delete debugButton;
    delete newGameButton;
    delete testButton1;
    delete testButton2;
}
