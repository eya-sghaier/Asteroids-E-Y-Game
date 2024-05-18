#include "Entity.h"
#include <cmath>

Entity::Entity() {
    life = true;
}

void Entity::settings(Animation& a, int X, int Y, float Angle, int radius) {
    anim = a;
    x = X;
    y = Y;
    angle = Angle;
    R = radius;
}

void Entity::draw(sf::RenderWindow& app) {
    anim.sprite.setPosition(x, y);
    anim.sprite.setRotation(angle + 90);
    app.draw(anim.sprite);
}

bool isCollide(Entity* a, Entity* b) {
    return (b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y) < (a->R + b->R) * (a->R + b->R);
}