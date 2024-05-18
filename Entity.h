#pragma once

#include "Animation.h"

class Entity {
public:
    float x, y, dx, dy, R, angle;
    bool life;
    std::string name;
    Animation anim;

    Entity();
    void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1);
    void update(); // Ajout de la fonction update
    void draw(sf::RenderWindow& app);
    virtual ~Entity() {}

private:
  Entity() : x(0), y(0), dx(0), dy(0), R(0), angle(0), life(true) {}
};

bool isCollide(Entity* a, Entity* b);