// main.cpp
// This is where the game starts.
// I tried to keep this file simple - just sets up the game and runs the loop.

#include <iostream>
#include <string>
#include "Game.h"

int main()
{
    Game game;
    game.init();

    // The game loop!
    // Each "turn" the player types a command, then the world updates.
    // It's turn-based instead of real-time because doing real-time input
    // in a terminal without a library is really complicated.
    // This still shows all the same game mechanics though.

    // I use a fake deltaTime since it's turn-based
    // Each "turn" counts as 0.5 seconds of in-game time
    float fakeDeltaTime = 0.5f;

    while (game.running)
    {
        // show what's happening
        game.render();

        // get player input
        std::string inputLine;
        std::getline(std::cin, inputLine);

        // if input is empty (player just pressed enter) skip it
        if (inputLine.empty())
        {
            game.update(fakeDeltaTime);
            continue;
        }

        // only look at the first character they typed
        char key = inputLine[0];

        // handle what the player typed
        game.handleInput(key, fakeDeltaTime);

        // update the game world (move zombies, check health, etc.)
        // even if player did nothing, time passes
        game.update(fakeDeltaTime);
    }

    std::cout << "\nThanks for playing!" << std::endl;

    return 0;
}
