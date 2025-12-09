#include "Duck.h"

Duck::Duck(const sf::Vector2f& pos)
    : alive(true), velocity(100.f, 0.f)
{
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(pos);
}

void Duck::update(float dt)
{
    if (!alive) return;
    sf::Vector2f p = shape.getPosition();
    p += velocity * dt;
    // simple horizontal wrap
    if (p.x > 800 + shape.getRadius()) p.x = -shape.getRadius();
    if (p.x < -shape.getRadius()) p.x = 800 + shape.getRadius();
    shape.setPosition(p);
}

void Duck::draw(sf::RenderWindow& window)
{
    if (!alive) return;
    window.draw(shape);
}

bool Duck::isHit(const sf::Vector2f& point) const
{
    if (!alive) return false;
    sf::Vector2f p = shape.getPosition();
    float dx = p.x - point.x;
    float dy = p.y - point.y;
    float r = shape.getRadius();
    return (dx*dx + dy*dy) <= (r*r);
}

bool Duck::isAlive() const { return alive; }
void Duck::kill() { alive = false; }
sf::Vector2f Duck::getPosition() const { return shape.getPosition(); }
