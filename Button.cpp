#include "Button.h"

Button::Button(sf::Vector2f _position, std::function<void(void)> _onClick) {
    position = _position;
    on_click = _onClick;
}

//Basic getters and setters
sf::Vector2f Button::getPosition() {return position;};
sf::Sprite* Button::getSprite() {return sprite;}
void Button::setSprite(sf::Sprite* _sprite) {sprite = _sprite;}
void Button::onClick() {on_click();}

Button::~Button() {delete sprite;}