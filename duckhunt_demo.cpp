#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <string>

#include "Duck.h"
#include "GameManager.h"

// Simple demo with sprite/audio integration, dynamic spawn, scoring and restart
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "DuckHunt Demo");
    window.setFramerateLimit(60);

    // load font
    sf::Font font;
    bool fontLoaded = true;
    if (!font.loadFromFile("./assets/fonts/Minecraft.ttf"))
    {
        fontLoaded = false;
        std::cerr << "Warning: could not load font ./assets/fonts/Minecraft.ttf\n";
    }

    // load music/background
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("./assets/music/bg.ogg"))
    {
        std::cerr << "Info: background music not found at ./assets/music/bg.ogg\n";
    }
    else
    {
        bgMusic.setLoop(true);
        bgMusic.play();
    }

    // sound effects
    sf::SoundBuffer shotBuf, hitBuf;
    sf::Sound shotSound, hitSound;
    if (shotBuf.loadFromFile("./assets/sfx/shot.wav")) { shotSound.setBuffer(shotBuf); }
    else { std::cerr << "Info: shot SFX not found at ./assets/sfx/shot.wav\n"; }
    if (hitBuf.loadFromFile("./assets/sfx/hit.wav")) { hitSound.setBuffer(hitBuf); }
    else { std::cerr << "Info: hit SFX not found at ./assets/sfx/hit.wav\n"; }

    // random generator for spawn positions
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> yDist(50.f, 550.f);
    std::uniform_int_distribution<int> dirDist(0, 1);

    std::vector<Duck> ducks;

    GameManager game(3, 5); // 3 lives, 5 bullets

    sf::Text info;
    if (fontLoaded) info.setFont(font);
    info.setCharacterSize(18);
    info.setFillColor(sf::Color::White);
    info.setPosition(10.f, 10.f);

    sf::Text centerText;
    if (fontLoaded) centerText.setFont(font);
    centerText.setCharacterSize(36);
    centerText.setFillColor(sf::Color::Red);
    centerText.setPosition(200.f, 260.f);

    // transient hit/miss HUD text
    sf::Text hitMissText;
    if (fontLoaded) hitMissText.setFont(font);
    hitMissText.setCharacterSize(24);
    hitMissText.setPosition(10.f, 40.f);
    hitMissText.setFillColor(sf::Color::White);
    float hitMissTimer = 0.f;
    const float hitMissDuration = 1.0f; // seconds

    // try to preload duck texture path
    const std::string duckTexPath = "./assets/images/duck.png";

    sf::Clock clock;
    float spawnTimer = 0.f;
    const float spawnInterval = 2.0f; // seconds

    // initial spawn
    auto spawnDuck = [&](float x, float y, float vx) {
        Duck d(sf::Vector2f(x, y));
        if (d.loadTexture(duckTexPath)) {
            // texture loaded inside duck
        }
        d.respawn(sf::Vector2f(x, y), sf::Vector2f(vx, 0.f));
        ducks.push_back(std::move(d));
    };

    // spawn a few initial ducks
    spawnDuck(100.f, 200.f, 80.f);
    spawnDuck(300.f, 300.f, -120.f);

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
                // play shot sound if available
                if (shotSound.getBuffer()) shotSound.play();
                bool hit = game.processShot(worldPos, ducks);
                if (hit && hitSound.getBuffer()) hitSound.play();
                std::cout << (hit ? "hit!\n" : "miss!\n") << std::flush;
                // set HUD message
                if (fontLoaded)
                {
                    if (hit)
                        hitMissText.setString("hit!");
                    else
                        hitMissText.setString("miss!");
                    hitMissText.setFillColor(hit ? sf::Color::Green : sf::Color::Red);
                    hitMissTimer = hitMissDuration;
                }
            }

            // restart on R when game over
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && game.getLives() == 0)
            {
                // reset game
                game.reset(3, 5);
                ducks.clear();
                spawnDuck(100.f, 200.f, 80.f);
                spawnDuck(300.f, 300.f, -120.f);
                spawnTimer = 0.f;
            }
        }

        float dt = clock.restart().asSeconds();
        spawnTimer += dt;

        // spawn periodically
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.f;
            float y = yDist(rng);
            bool fromLeft = dirDist(rng) == 0;
            if (fromLeft)
                spawnDuck(-20.f, y, 80.f + (std::rand() % 80));
            else
                spawnDuck(820.f, y, - (80.f + (std::rand() % 80)));
        }

        for (auto& d : ducks) d.update(dt);

        // HUD string
        std::string hud = "Lives: " + std::to_string(game.getLives()) + "  Bullets: " + std::to_string(game.getBullets()) + "  Score: " + std::to_string(game.getScore());
        info.setString(hud);

        window.clear(sf::Color::Black);
        for (auto& d : ducks) d.draw(window);
        window.draw(info);

        // update and draw transient hit/miss HUD
        if (hitMissTimer > 0.f)
        {
            hitMissTimer -= dt;
            if (hitMissTimer < 0.f) hitMissTimer = 0.f;
            if (fontLoaded) window.draw(hitMissText);
        }

        if (game.getLives() == 0)
        {
            centerText.setString("GAME OVER\nPress R to restart");
            window.draw(centerText);
        }

        window.display();
    }

    return 0;
}
