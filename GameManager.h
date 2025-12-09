#pragma once

#include <SFML/System.hpp>
#include <vector>

class Duck;

class GameManager {
public:
    GameManager(int initialLives = 3, int initialBullets = 3);

    // Process a shot at position shotPos. If a duck is hit, returns true.
    // If no duck is hit, decrement lives or bullets and return false.
    bool processShot(const sf::Vector2f& shotPos, std::vector<Duck>& ducks);

    void decrementLives(int amount = 1);
    void decrementBullets(int amount = 1);

    int getLives() const;
    int getBullets() const;

private:
    int lives;
    int bullets;
};
