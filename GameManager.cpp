#include "GameManager.h"
#include "Duck.h"

GameManager::GameManager(int initialLives, int initialBullets)
    : lives(initialLives), bullets(initialBullets)
{
}

bool GameManager::processShot(const sf::Vector2f& shotPos, std::vector<Duck>& ducks)
{
    // check ducks for hit
    for (auto& d : ducks)
    {
        if (!d.isAlive()) continue;
        if (d.isHit(shotPos))
        {
            d.kill();
            return true;
        }
    }

    // no duck hit -> decrement bullets if used, otherwise lives
    if (bullets > 0)
        decrementBullets(1);
    else
        decrementLives(1);

    return false;
}

void GameManager::decrementLives(int amount)
{
    lives -= amount;
    if (lives < 0) lives = 0;
}

void GameManager::decrementBullets(int amount)
{
    bullets -= amount;
    if (bullets < 0) bullets = 0;
}

int GameManager::getLives() const { return lives; }
int GameManager::getBullets() const { return bullets; }
