#pragma once
#include "Entity.h"
#include "Animation.h"

class Explosion : public Entity {
public:
    Explosion(Animation& anim, float x, float y) 
    {
        settings(anim, x, y);
        name = "explosion";
    }

    void update() {
        if (anim.isEnd()) {
            life = false;
        }
    }
};