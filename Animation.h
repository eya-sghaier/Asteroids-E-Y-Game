#pragma once

#include <SFML/Graphics.hpp>

class Animation {
public:
    float Frame, speed;
    std::vector<sf::IntRect> frames;
    sf::Sprite sprite;

    Animation();
    Animation(sf::Texture& texture, int x, int y, int w, int h, int count, float speed);
    void update();
    bool isEnd() ; // Ajout du const pour les fonctions membres constantes
};