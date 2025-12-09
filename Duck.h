#pragma once

#include <SFML/Graphics.hpp>

class Duck {
public:
    Duck(const sf::Vector2f& pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isHit(const sf::Vector2f& point) const;
    bool isAlive() const;
    void kill();
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    bool alive;
    sf::Vector2f velocity;
};
