// Game.cpp
// This is the main game logic file. Everything comes together here.
// The update() function is called every frame and does all the work.

#include "Game.h"
#include <iostream>
#include <cmath>
#include <cstdlib>   // for rand()
#include <ctime>     // for seeding rand
#include <algorithm> // for remove_if

// I defined this in Player.cpp but need it here too
// Should probably put it in a utils file but this is fine for now
static float toRad(float deg)
{
    return deg * (3.14159265f / 180.0f);
}

Game::Game()
{
    running = true;
    spawnTimer = 0.0f;
    spawnInterval = 3.0f;  // spawn a zombie every 3 seconds
    totalTime = 0.0f;
}

void Game::init()
{
    // seed random so zombies spawn in different places each run
    srand((unsigned int)time(0));

    std::cout << "=== ZOMBIE SHOOTER ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W = move forward" << std::endl;
    std::cout << "  S = move backward" << std::endl;
    std::cout << "  A = strafe left" << std::endl;
    std::cout << "  D = strafe right" << std::endl;
    std::cout << "  Q = turn left" << std::endl;
    std::cout << "  E = turn right" << std::endl;
    std::cout << "  F = shoot" << std::endl;
    std::cout << "  X = quit" << std::endl;
    std::cout << "========================" << std::endl;

    // spawn a couple zombies at the start so it's not empty
    spawnZombie();
    spawnZombie();
}

void Game::update(float dt)
{
    if (!running) return;

    totalTime += dt;

    // tick down the player's shoot cooldown
    player.updateCooldown(dt);

    // move all zombies toward the player
    for (int i = 0; i < (int)zombies.size(); i++)
    {
        zombies[i].moveToward(player.x, player.z, dt);
    }

    // check if zombies are touching the player
    checkZombieCollisions();

    // spawn new zombies on a timer
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        spawnZombie();

        // over time spawn faster, minimum 1 second between spawns
        if (spawnInterval > 1.0f)
            spawnInterval -= 0.1f;
    }

    // remove any zombies that died
    cleanupDeadZombies();

    // check if player died
    if (!player.isAlive())
    {
        running = false;
        std::cout << "\n*** YOU DIED ***" << std::endl;
        std::cout << "Final score: " << player.score << std::endl;
        std::cout << "Survived for: " << (int)totalTime << " seconds" << std::endl;
    }
}

void Game::spawnZombie()
{
    // spawn somewhere around the player but not too close
    // pick a random angle and distance
    float spawnAngle = (float)(rand() % 360);
    float spawnDist = 15.0f + (float)(rand() % 10);  // 15 to 25 units away

    float spawnX = player.x + sin(toRad(spawnAngle)) * spawnDist;
    float spawnZ = player.z + cos(toRad(spawnAngle)) * spawnDist;

    zombies.push_back(Zombie(spawnX, spawnZ));
}

bool Game::checkBulletHit()
{
    // Simple shooting: check what's directly in front of the player
    // We cast a "ray" forward in small steps and see if it hits a zombie

    float rayX = player.x;
    float rayZ = player.z;

    // direction the player is facing
    float dirX = sin(toRad(player.angle));
    float dirZ = cos(toRad(player.angle));

    float stepSize = 0.5f;  // check every 0.5 units
    float maxRange = 20.0f; // can't hit things farther than this

    float traveled = 0.0f;

    while (traveled < maxRange)
    {
        // move the ray forward one step
        rayX += dirX * stepSize;
        rayZ += dirZ * stepSize;
        traveled += stepSize;

        // check if this ray position is inside any zombie
        for (int i = 0; i < (int)zombies.size(); i++)
        {
            if (!zombies[i].alive) continue;

            float dx = rayX - zombies[i].x;
            float dz = rayZ - zombies[i].z;
            float dist = sqrt(dx*dx + dz*dz);

            // zombie "hitbox" is 1 unit radius
            if (dist < 1.0f)
            {
                zombies[i].hit();
                player.score++;
                std::cout << "  [HIT! Score: " << player.score << "]" << std::endl;
                return true;
            }
        }
    }

    std::cout << "  [missed]" << std::endl;
    return false;
}

void Game::checkZombieCollisions()
{
    for (int i = 0; i < (int)zombies.size(); i++)
    {
        if (!zombies[i].alive) continue;

        float dx = player.x - zombies[i].x;
        float dz = player.z - zombies[i].z;
        float dist = sqrt(dx*dx + dz*dz);

        // if zombie is within 1.5 units of player, it's touching
        if (dist < 1.5f)
        {
            // zombie bites the player
            // Using a fixed damage per second approach: 10 damage per second
            // but we're checking every frame so we just do a small amount
            // Actually let me just do 1 damage per frame they're touching
            // This is a bit frame-rate dependent but simple enough
            player.takeDamage(1);

            // small push: move zombie back slightly so it doesn't just
            // stand inside the player dealing tons of damage instantly
            // actually let's skip that, it complicates things
        }
    }
}

void Game::cleanupDeadZombies()
{
    // go backwards through the list and remove dead ones
    // I do it backwards so removing one doesn't mess up the index
    for (int i = (int)zombies.size() - 1; i >= 0; i--)
    {
        if (!zombies[i].alive)
        {
            zombies.erase(zombies.begin() + i);
        }
    }
}

void Game::render()
{
    // We don't have a real graphics window, so print the game state
    // This is like a mini status display

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Health: " << player.health
              << "  Score: " << player.score
              << "  Zombies alive: " << zombies.size() << std::endl;
    std::cout << "Position: (" << (int)player.x << ", " << (int)player.z << ")"
              << "  Facing: " << (int)player.angle << " deg" << std::endl;
    std::cout << "Time: " << (int)totalTime << "s" << std::endl;
    std::cout << "-----------------------------" << std::endl;

    // show nearby zombies with a very basic "radar"
    std::cout << "Nearby threats: ";
    int shownCount = 0;
    for (int i = 0; i < (int)zombies.size(); i++)
    {
        float dx = player.x - zombies[i].x;
        float dz = player.z - zombies[i].z;
        float dist = sqrt(dx*dx + dz*dz);
        if (dist < 10.0f)
        {
            std::cout << "[Z @" << (int)dist << "u] ";
            shownCount++;
        }
    }
    if (shownCount == 0) std::cout << "none close";
    std::cout << std::endl;

    std::cout << "Enter command (w/a/s/d/q/e=move, f=shoot, x=quit): ";
}

void Game::handleInput(char key, float dt)
{
    // using a fixed timestep for input so movement feels consistent
    float moveTime = 0.3f;  // treat each keypress as 0.3 seconds of movement

    switch (key)
    {
        case 'w': case 'W':
            player.moveForward(moveTime);
            break;
        case 's': case 'S':
            player.moveBackward(moveTime);
            break;
        case 'a': case 'A':
            player.moveLeft(moveTime);
            break;
        case 'd': case 'D':
            player.moveRight(moveTime);
            break;
        case 'q': case 'Q':
            player.turnLeft(moveTime);
            break;
        case 'e': case 'E':
            player.turnRight(moveTime);
            break;
        case 'f': case 'F':
            if (player.shoot())
                checkBulletHit();
            else
                std::cout << "  [reloading...]" << std::endl;
            break;
        case 'x': case 'X':
            running = false;
            std::cout << "Quit game. Final score: " << player.score << std::endl;
            break;
        default:
            std::cout << "  [unknown key]" << std::endl;
            break;
    }
}
