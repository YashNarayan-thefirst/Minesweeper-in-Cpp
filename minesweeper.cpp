#include "minesweeper.h"
#include "Button.h"
#include "ToolBox.h"


void toggleDebugMode() {
    auto& t = ToolBox::getInstance();
    t.debug_mode = !t.debug_mode;
}

bool getDebugMode() { return ToolBox::getInstance().debug_mode; }

void restart() {
    auto& t = ToolBox::getInstance();
    delete t.gameState;
    t.gameState = new GameState();
    if (getDebugMode()) toggleDebugMode();
}

void render() {
    auto& t = ToolBox::getInstance();
    const int flags = t.gameState->getMineCount() - t.gameState->getFlagCount();
    bool neg = std::abs(t.gameState->getMineCount());

    // Calculate digits
    int digs[] = {flags % 10, (flags / 10) % 10,0};

    // Map digit sprites
    sf::IntRect rects[] = {
        {0, 0, 21, 32}, {21, 0, 21, 32}, {42, 0, 21, 32}, {63, 0, 21, 32},
        {84, 0, 21, 32}, {105, 0, 21, 32}, {126, 0, 21, 32}, {147, 0, 21, 32},
        {168, 0, 21, 32}, {189, 0, 21, 32}, {210, 0, 21, 32}  // Last is negative
    };

    sf::Sprite sps[] = {sf::Sprite(t.digits), sf::Sprite(t.digits), sf::Sprite(t.digits)};
    for (int i = 0; i < 3; i++) sps[i].setTextureRect(rects[i == 2 && neg ? 10 : digs[i]]);
    sps[0].setPosition(42, 512), sps[1].setPosition(21, 512), sps[2].setPosition(0, 512);
    for (auto& s : sps) t.window.draw(s);

    // Set button sprites
    sf::Sprite* btnSprites[] = {
        new sf::Sprite(t.debug), new sf::Sprite(t.test1), new sf::Sprite(t.test2)
    };
    t.debugButton->setSprite(btnSprites[0]);
    t.testButton1->setSprite(btnSprites[1]);
    t.testButton2->setSprite(btnSprites[2]);

    // Set new game button based on state
    t.newGameButton->setSprite(new sf::Sprite(
        t.gameState->getPlayStatus() == GameState::PLAYING? t.happy: (t.gameState->getPlayStatus() == GameState::WIN ? t.win : t.lose)));

    // Set button positions
    Button* btns[] = {t.debugButton, t.newGameButton, t.testButton1, t.testButton2};
    for (auto* b : btns)
        b->getSprite()->setPosition(b->getPosition().x, b->getPosition().y);
    for (auto* b : btns) t.window.draw(*b->getSprite());
    for (int j = 0; j < t.board.size(); j++)
        for (int k = 0; k < t.board[0].size(); k++) t.gameState->getTile(k, j)->draw();
}

int launch() {
    auto& t = ToolBox::getInstance();
    t.gameState = new GameState();

    // Setup buttons
    t.debugButton = new Button({496, 512}, toggleDebugMode);
    t.newGameButton = new Button({368, 512}, restart);

    auto loadTest = [](const char* path) {
        auto& tb = ToolBox::getInstance();
        delete tb.gameState;
        tb.gameState = new GameState(path);
    };

    t.testButton1 = new Button({560, 512}, [&loadTest]{ loadTest("../boards/testboard1.brd"); });
    t.testButton2 = new Button({624, 512}, [&loadTest]{ loadTest("../boards/testboard2.brd"); });
    t.gameLoop();
    return 0;
}

int main() { return launch(); }
