#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Duck {
public:
    Duck(const sf::Vector2f& pos);
    // attempt to load a texture for sprite animation; returns true if loaded
    bool loadTexture(const std::string& path);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isHit(const sf::Vector2f& point) const;
    bool isAlive() const;
    void kill();
    sf::Vector2f getPosition() const;

    // respawn the duck at a position with a velocity
    void respawn(const sf::Vector2f& pos, const sf::Vector2f& vel);

private:
    // fallback shape if no texture available
    sf::CircleShape shape;
    // sprite + texture when available
    sf::Sprite sprite;
    sf::Texture texture;
    bool hasTexture = false;

    bool alive = true;
    sf::Vector2f velocity;

    // simple animation timer (for flapping or frame switching if desired)
    float animTimer = 0.f;
    float animInterval = 0.12f;
};
