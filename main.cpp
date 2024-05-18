#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <list>
#include "Animation.h"
#include "Asteroid.h"
#include "Player.h"
#include "Bullet.h"
#include "Entity.h"
#include "Explosion.h"

const int W = 1200;
const int H = 550;

void resetGame(std::list<Entity*>& entities, Player*& p, Animation& sPlayer, Animation& sRock, int& score) {
    for (auto it = entities.begin(); it != entities.end();) {
        if ((*it)->name != "player") {
            delete* it;
            it = entities.erase(it);
        }
        else {
            ++it;
        }
    }

    p->x = W / 2;
    p->y = H / 2;
    p->dx = 0;
    p->dy = 0;
    p->angle = 0;
    p->thrust = false;

    for (int i = 0; i < 15; i++) {
        Asteroid* a = new Asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }

    score = 0;
}

int main() {
    srand(time(0));

    sf::RenderWindow app(sf::VideoMode(W, H), "Our Game: Asteroids!");
    app.setFramerateLimit(60);

    sf::Texture t1, t2, t3, t4, t5, t6, t7;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/backR.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_red.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    t1.setSmooth(true);
    t2.setSmooth(true);

    sf::Sprite background(t2);
    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 30, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);

    std::list<Entity*> entities;

    for (int i = 0; i < 5; i++) {
        Asteroid* a = new Asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }

    Player* p = new Player();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    int score = 0;
    sf::Font font;
    font.loadFromFile("font/RoughenCornerRegular.ttf");
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    bool gameOver = false;
    sf::Texture gameoverTexture;
    gameoverTexture.loadFromFile("images/gameover.png");
    sf::Sprite gameoverSprite(gameoverTexture);
    gameoverSprite.setPosition(W / 2 - gameoverSprite.getLocalBounds().width / 2, H / 2 - gameoverSprite.getLocalBounds().height / 2);

    sf::SoundBuffer backgroundBuffer, gameoverBuffer, fireBuffer;
    backgroundBuffer.loadFromFile("audio/background_Music.wav");
    gameoverBuffer.loadFromFile("audio/game_over.wav");
    fireBuffer.loadFromFile("audio/fire.wav");

    sf::Sound backgroundSound, gameoverSound, fireSound;
    backgroundSound.setBuffer(backgroundBuffer);
    gameoverSound.setBuffer(gameoverBuffer);
    fireSound.setBuffer(fireBuffer);

    float volumeLevel = 70;
    float volumeLevelF = 40;
    backgroundSound.setVolume(volumeLevel);
    gameoverSound.setVolume(volumeLevel);
    fireSound.setVolume(volumeLevelF);

    bool gameStarted = false;

    while (app.isOpen()) {
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                app.close();

            if (!gameStarted && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                gameStarted = true;
                backgroundSound.setLoop(true);
                backgroundSound.play();
            }

            if (gameStarted && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    Bullet* b = new Bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                    fireSound.play();
                }
                if (gameOver && event.key.code == sf::Keyboard::Return) {
                    resetGame(entities, p, sPlayer, sRock, score);
                    gameOver = false;
                    backgroundSound.play();
                }
            }
        }

        if (gameStarted) {
            if (!gameOver) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p->angle += 3;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  p->angle -= 3;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) p->thrust = true;
                else p->thrust = false;

                for (auto a : entities)
                    for (auto b : entities) {
                        if (a->name == "asteroid" && b->name == "bullet")
                            if (isCollide(a, b)) {
                                a->life = false;
                                b->life = false;

                                Explosion* e = new Explosion(sExplosion, a->x, a->y); // Use Explosion class
                                entities.push_back(e);
                                for (int i = 0; i < 2; i++) {
                                    if (a->R == 15) continue;
                                    Asteroid* e = new Asteroid();
                                    e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                                    entities.push_back(e);
                                }
                                score++;
                            }
                        if (a->name == "player" && b->name == "asteroid")
                            if (isCollide(a, b)) {
                                b->life = false;

                                Explosion* e = new Explosion(sExplosion_ship, a->x, a->y); // Use Explosion class
                                entities.push_back(e);

                                gameOver = true;
                                backgroundSound.stop();
                                gameoverSound.play();
                            }
                    }

                if (p->thrust)  p->anim = sPlayer_go;
                else   p->anim = sPlayer;

                for (auto e : entities)
                    if (e->name == "explosion")
                        if (e->anim.isEnd()) e->life = false;

                if (rand() % 150 == 0) {
                    Asteroid* a = new Asteroid();
                    a->settings(sRock, 0, rand() % H, rand() % 360, 25);
                    entities.push_back(a);
                }

                for (auto i = entities.begin(); i != entities.end();) {
                    Entity* e = *i;
                    e->update();
                    if (e->life == false) {
                        i = entities.erase(i);
                        delete e;
                    }
                    else ++i;
                }

                scoreText.setString("Score: " + std::to_string(score));
                app.draw(background);
                app.draw(scoreText);
                for (auto i : entities) i->draw(app);

                app.display();
            }
            else {
                app.draw(gameoverSprite);
                app.display();
            }
        }
        else {
            sf::Text startText;
            startText.setFont(font);
            startText.setCharacterSize(30);
            startText.setFillColor(sf::Color::White);
            startText.setString("Press Enter Key");
            startText.setPosition(W / 2 - startText.getLocalBounds().width / 2, H / 2 - startText.getLocalBounds().height / 2);

            app.draw(startText);
            app.display();
        }
    }

    return 0;
}