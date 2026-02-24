#ifndef GAME_H
#define GAME_H

// Game.h
// The Game class runs everything. It has the player, all the zombies,
// and controls the game loop logic.

#include "Player.h"
#include "Zombie.h"
#include <vector>
#include <string>

class Game
{
public:
    Player player;

    // all the zombies currently in the world
    // I use a vector because the number of zombies changes
    std::vector<Zombie> zombies;

    // is the game still running?
    bool running;

    // timer for spawning new zombies
    float spawnTimer;
    float spawnInterval;  // how many seconds between spawns

    // total time elapsed
    float totalTime;

    // constructor
    Game();

    // called once to set things up
    void init();

    // called every frame - dt is seconds since last frame
    void update(float dt);

    // spawn a new zombie somewhere around the player
    void spawnZombie();

    // check if a bullet (shot from player) hits any zombie
    // returns true if something was hit
    bool checkBulletHit();

    // check if any zombie is touching the player
    void checkZombieCollisions();

    // remove dead zombies from the list
    void cleanupDeadZombies();

    // print the current game state to terminal
    void render();

    // process keyboard input
    // I'm using a simple char here to represent what key is pressed
    void handleInput(char key, float dt);
};

#endif
