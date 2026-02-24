// Zombie.cpp
// Zombie behavior - it's pretty dumb, just walks toward the player.
// That's all it does!

#include "Zombie.h"
#include <cmath>  // for sqrt

Zombie::Zombie(float startX, float startZ)
{
    x = startX;
    y = 0.0f;
    z = startZ;
    health = 1;  // one shot one kill for simplicity
    alive = true;
}

void Zombie::moveToward(float playerX, float playerZ, float deltaTime)
{
    if (!alive) return;

    float speed = 2.0f;  // zombies are slow

    // figure out direction from zombie to player
    float dx = playerX - x;
    float dz = playerZ - z;

    // get the distance so we can normalize the direction
    float dist = sqrt(dx * dx + dz * dz);

    // avoid dividing by zero if zombie is right on top of player
    if (dist < 0.01f) return;

    // normalize: divide by distance to get a unit vector
    float dirX = dx / dist;
    float dirZ = dz / dist;

    // move in that direction
    x += dirX * speed * deltaTime;
    z += dirZ * speed * deltaTime;
}

void Zombie::hit()
{
    // one bullet kills it
    health = 0;
    alive = false;
}
