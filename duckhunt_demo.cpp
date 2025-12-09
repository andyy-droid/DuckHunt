#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Duck.h"
#include "GameManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "DuckHunt Demo");
    window.setFramerateLimit(60);

    // load font if available
    sf::Font font;
    if (!font.loadFromFile("./assets/fonts/Minecraft.ttf"))
    {
        // fallback to default font: text will not display if not found, but demo still runs
    }

    std::vector<Duck> ducks;
    ducks.emplace_back(sf::Vector2f(100.f, 200.f));
    ducks.emplace_back(sf::Vector2f(300.f, 300.f));
    ducks.emplace_back(sf::Vector2f(600.f, 150.f));

    GameManager game(3, 5); // 3 lives, 5 bullets

    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(18);
    info.setFillColor(sf::Color::White);
    info.setPosition(10.f, 10.f);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                bool hit = game.processShot(worldPos, ducks);
                std::cout << (hit ? "Hit!\n" : "Miss!\n");
            }
        }

        float dt = clock.restart().asSeconds();

        for (auto& d : ducks) d.update(dt);

        info.setString("Lives: " + std::to_string(game.getLives()) + "  Bullets: " + std::to_string(game.getBullets()));

        window.clear(sf::Color::Black);
        for (auto& d : ducks) d.draw(window);
        window.draw(info);
        window.display();
    }

    return 0;
}
