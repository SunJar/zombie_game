#ifndef PLAYER_H
#define PLAYER_H

// Player.h
// This file describes what a Player is in our game.
// I made a simple class to hold the player's position, health, and score.

class Player
{
public:
    // position in the world
    float x;
    float y;  // y is up/down but we won't really use it much
    float z;

    // how much health the player has
    int health;

    // how many zombies the player has killed
    int score;

    // which direction the player is facing (in degrees, around Y axis)
    float angle; // 0 = facing +Z direction

    // constructor - sets starting values
    Player();

    // move the player based on WASD input
    // deltaTime makes movement speed consistent regardless of framerate
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    // turn the player left or right
    void turnLeft(float deltaTime);
    void turnRight(float deltaTime);

    // take damage from a zombie
    void takeDamage(int amount);

    // check if player is alive
    bool isAlive();

    // tick down the shoot cooldown every frame
    void updateCooldown(float deltaTime);

    // shoot - returns true if player can shoot right now
    // has a small cooldown so you can't spam bullets
    bool shoot();

private:
    // cooldown timer for shooting
    float shootCooldown;

    // movement speed
    float moveSpeed;

    // turn speed
    float turnSpeed;
};

#endif
