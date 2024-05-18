#include "Asteroid.h"
const int W = 1200;
const int H = 550;

Asteroid::Asteroid() {
    dx = rand() % 5 - 2;
    dy = rand() % 5 - 2;
    name = "asteroid";
}

void Asteroid::update() {
    x += dx;
    y += dy;

    if (x > W) x = 0;  if (x < 0) x = W;
    if (y > H) y = 0;  if (y < 0) y = H;
}