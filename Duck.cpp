#include "Duck.h"
#include <iostream>

Duck::Duck(const sf::Vector2f& pos)
    : hasTexture(false), alive(true), velocity(100.f, 0.f)
{
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(pos);

    // Try to load a default duck texture from the assets folder; if present,
    // the sprite will replace the yellow circle automatically.
    loadTexture("./assets/images/duck.png");
}

bool Duck::loadTexture(const std::string& path)
{
    if (texture.loadFromFile(path))
    {
        hasTexture = true;
        sprite.setTexture(texture);
        // center origin based on texture size
        sf::Vector2u sz = texture.getSize();
        sprite.setOrigin(sz.x / 2.f, sz.y / 2.f);
        // scale image to a desired visual size (height or width), e.g. 64px
        float desiredSize = 64.f;
        float maxDim = static_cast<float>(std::max(1u, std::max(sz.x, sz.y)));
        float scale = desiredSize / maxDim;
        sprite.setScale(scale, scale);
        // place sprite at current shape position
        sprite.setPosition(shape.getPosition());
        std::cerr << "Duck::loadTexture - loaded texture from: " << path << " (size=" << texture.getSize().x << "x" << texture.getSize().y << ")\n";
        return true;
    }
    else
    {
        hasTexture = false;
        std::cerr << "Duck::loadTexture - FAILED to load texture from: " << path << "\n";
        return false;
    }
}

void Duck::update(float dt)
{
    if (!alive) return;

    sf::Vector2f p = hasTexture ? sprite.getPosition() : shape.getPosition();
    p += velocity * dt;

    // simple horizontal wrap based on window width constants; keep same behavior as before
    float radius = hasTexture ? 20.f : shape.getRadius();
    if (p.x > 800 + radius) p.x = -radius;
    if (p.x < -radius) p.x = 800 + radius;

    if (hasTexture)
        sprite.setPosition(p);
    else
        shape.setPosition(p);

    // simple animation: bobbing and rotate a bit
    animTimer += dt;
    if (animTimer >= animInterval)
    {
        animTimer = 0.f;
        // flip a tiny vertical offset to simulate flapping
        if (hasTexture)
        {
            float offset = (std::rand() % 3) - 1; // -1,0,1
            sprite.move(0.f, offset);
        }
    }
}

void Duck::draw(sf::RenderWindow& window)
{
    if (!alive) return;
    if (hasTexture)
        window.draw(sprite);
    else
        window.draw(shape);
}

bool Duck::isHit(const sf::Vector2f& point) const
{
    if (!alive) return false;
    if (hasTexture)
    {
        // Use sprite bounding box for textured ducks (more accurate for images)
        return sprite.getGlobalBounds().contains(point);
    }
    else
    {
        sf::Vector2f p = shape.getPosition();
        float dx = p.x - point.x;
        float dy = p.y - point.y;
        float r = shape.getRadius();
        return (dx*dx + dy*dy) <= (r*r);
    }
}

bool Duck::isAlive() const { return alive; }
void Duck::kill() { alive = false; }
sf::Vector2f Duck::getPosition() const { return hasTexture ? sprite.getPosition() : shape.getPosition(); }

void Duck::respawn(const sf::Vector2f& pos, const sf::Vector2f& vel)
{
    alive = true;
    velocity = vel;
    if (hasTexture)
        sprite.setPosition(pos);
    else
        shape.setPosition(pos);
}
