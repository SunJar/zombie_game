#ifndef ZOMBIE_H
#define ZOMBIE_H

// Zombie.h
// A zombie just has a position and health.
// It moves toward the player and damages it on contact.

class Zombie
{
public:
    float x;
    float y;
    float z;

    int health;

    // is this zombie still alive? (false = should be removed)
    bool alive;

    // constructor - spawns zombie at a given position
    Zombie(float startX, float startZ);

    // move toward the player's position
    void moveToward(float playerX, float playerZ, float deltaTime);

    // take a hit from the player's bullet
    void hit();
};

#endif
