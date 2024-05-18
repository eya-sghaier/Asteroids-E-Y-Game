#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
public:
    Player();
    void update();
    bool thrust;
};

#endif // PLAYER_H