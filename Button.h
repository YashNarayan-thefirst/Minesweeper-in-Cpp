#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
    sf::Vector2f position;
    sf::Sprite* sprite;
    std::function<void(void)> on_click;
    public:
        Button(sf::Vector2f _position, std::function<void(void)> _onClick);
        sf::Vector2f getPosition();
        sf::Sprite* getSprite();
        void setSprite(sf::Sprite* _sprite);
        void onClick();

    ~Button();
};

