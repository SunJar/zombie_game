// Player.cpp
// This file has all the actual code for what the player does.
// I tried to keep it simple - just math for moving around.

#include "Player.h"
#include <cmath>  // for sin and cos when moving in a direction

// I define PI here because I need it for angle calculations
#define MY_PI 3.14159265f

Player::Player()
{
    // start in the middle of the world
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;

    health = 100;
    score = 0;
    angle = 0.0f;  // facing forward

    shootCooldown = 0.0f;
    moveSpeed = 5.0f;   // units per second
    turnSpeed = 90.0f;  // degrees per second
}

// helper: convert degrees to radians
// I keep forgetting the formula so I made a function
static float toRadians(float degrees)
{
    return degrees * (MY_PI / 180.0f);
}

void Player::moveForward(float deltaTime)
{
    // move in the direction the player is facing
    // sin and cos on the angle gives us the direction vector
    x += sin(toRadians(angle)) * moveSpeed * deltaTime;
    z += cos(toRadians(angle)) * moveSpeed * deltaTime;
}

void Player::moveBackward(float deltaTime)
{
    // just move the opposite direction
    x -= sin(toRadians(angle)) * moveSpeed * deltaTime;
    z -= cos(toRadians(angle)) * moveSpeed * deltaTime;
}

void Player::moveLeft(float deltaTime)
{
    // strafe left - 90 degrees to the left of facing direction
    x += sin(toRadians(angle - 90.0f)) * moveSpeed * deltaTime;
    z += cos(toRadians(angle - 90.0f)) * moveSpeed * deltaTime;
}

void Player::moveRight(float deltaTime)
{
    // strafe right
    x += sin(toRadians(angle + 90.0f)) * moveSpeed * deltaTime;
    z += cos(toRadians(angle + 90.0f)) * moveSpeed * deltaTime;
}

void Player::turnLeft(float deltaTime)
{
    angle -= turnSpeed * deltaTime;
}

void Player::turnRight(float deltaTime)
{
    angle += turnSpeed * deltaTime;
}

void Player::takeDamage(int amount)
{
    health -= amount;
    // don't let health go below 0
    if (health < 0)
        health = 0;
}

bool Player::isAlive()
{
    return health > 0;
}

void Player::updateCooldown(float deltaTime)
{
    if (shootCooldown > 0.0f)
        shootCooldown -= deltaTime;
}

bool Player::shoot()
{
    // can only shoot if cooldown is done
    if (shootCooldown <= 0.0f)
    {
        shootCooldown = 0.4f;  // wait 0.4 seconds before next shot
        return true;
    }
    return false;
}

// The cooldown ticks down inside shoot() when it returns false,
// but we also need to reduce it passively each frame.
// Game.cpp calls player.shoot() which handles this check already.
