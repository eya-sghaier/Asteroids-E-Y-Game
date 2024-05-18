#include "Bullet.h"
const int W = 1200;
const int H = 550;
const float DEGTORAD = 0.017453f;

Bullet::Bullet() {
    name = "bullet";
}

void Bullet::update() {
    dx = cos(angle * DEGTORAD) * 6;
    dy = sin(angle * DEGTORAD) * 6;
    x += dx;
    y += dy;

    if (x > W || x < 0 || y > H || y < 0) life = false;
}