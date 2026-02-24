# Zombie Shooter - Simple 3D Demo
### A beginner C++ project inspired by Left 4 Dead

---

## Project Structure

```
zombie_game/
├── main.cpp       <- starts the game, runs the game loop
├── Game.h         <- declares the Game class
├── Game.cpp       <- game logic: updating, spawning, collisions
├── Player.h       <- declares the Player class
├── Player.cpp     <- player movement, health, shooting
├── Zombie.h       <- declares the Zombie class
├── Zombie.cpp     <- zombie movement and hit detection
└── README.md      <- this file
```

I split it into files because my professor said to use multiple files. Each class gets its own .h (header) and .cpp (implementation) file. The header says what exists, the .cpp says how it works.

---

## What Each File Does

**main.cpp**
This is the entry point. It creates a Game object, calls init() to set things up, then runs the game loop. The loop shows the game state, reads a keypress from the player, handles it, and updates the world. Repeat until game over.

**Player.h / Player.cpp**
The Player class holds everything about the player: position (x, y, z), health, score, and facing angle. It has methods for moving in each direction, turning, taking damage, and shooting. Movement uses sin/cos math to move in the direction the player is currently facing.

**Zombie.h / Zombie.cpp**
The Zombie class is pretty simple. It has a position and an alive flag. Its main job is moveToward() which calculates the direction from itself to the player and moves that way. It has a hit() function that kills it.

**Game.h / Game.cpp**
This is the big one. The Game class owns the Player object and a vector of Zombies. It coordinates everything: spawning zombies on a timer, moving them, checking if they hit the player, handling bullet checks when the player shoots, and cleaning up dead zombies.

---

## How the Game Loop Works

The game loop is in main.cpp. It's **turn-based** (not real-time) because doing real-time keyboard input in a terminal without extra libraries is complicated.

Each iteration ("turn"):
1. `game.render()` - prints the current game state and a prompt
2. Player types a key and presses Enter
3. `game.handleInput(key, dt)` - player moves, shoots, etc.
4. `game.update(dt)` - zombies move, collisions are checked, new ones spawn
5. Repeat

Each turn counts as 0.5 seconds of in-game time (`fakeDeltaTime = 0.5f`). This means zombies move a bit each turn, and the spawn timer ticks down each turn.

---

## How WASD Movement Works

The player has a facing angle (in degrees). When you press W, it moves forward in the direction it's facing. The math uses sin and cos:

```
x += sin(angle_in_radians) * speed * deltaTime;
z += cos(angle_in_radians) * speed * deltaTime;
```

- W moves forward (positive direction)
- S moves backward (negative direction)
- A strafes left (angle - 90 degrees)
- D strafes right (angle + 90 degrees)
- Q turns left (decreases angle)
- E turns right (increases angle)

The `deltaTime` is used to make movement frame-rate independent (even if the game runs slower, you move the same distance per second).

---

## How Shooting Works

Press **F** to shoot.

There's a cooldown of 0.4 seconds between shots (tracked by `shootCooldown` in Player.cpp). If you try to shoot too fast, it says "reloading...".

When you do shoot, `checkBulletHit()` in Game.cpp is called. It simulates a bullet by starting at the player's position and moving forward in small steps (0.5 units per step). At each step, it checks if it's within 1 unit of any zombie. If yes, that zombie dies and your score goes up.

Maximum bullet range is 20 units. If nothing is hit, it prints "missed".

---

## How Zombie Movement Works

Every turn, `update()` calls `moveToward(player.x, player.z, dt)` on every zombie.

Inside moveToward():
1. Calculate the difference in X and Z between the zombie and player
2. Calculate the actual distance using sqrt(dx*dx + dz*dz)
3. Divide dx and dz by distance to get a direction with length 1 (normalization)
4. Multiply by speed and deltaTime, add to zombie's position

This makes every zombie walk straight at the player, no matter where they are.

---

## How Health and Game Over Works

**Taking damage:** When a zombie gets within 1.5 units of the player, `takeDamage(1)` is called. Since each turn counts as 0.5 seconds and this check runs each turn, standing next to a zombie will drain health quickly.

**Health display:** Health is shown each turn in the status display.

**Game over:** In `update()`, after all the zombie stuff, it checks `player.isAlive()`. If health is 0 or below, `running` is set to false, a "YOU DIED" message is printed with your final score and survival time, and the loop ends.

There is no win condition - zombies keep spawning forever and spawn faster over time. Survive as long as you can.

---

## How to Compile and Run

You need g++ installed. Open a terminal in the zombie_game folder.

**Compile all files at once:**
```bash
g++ main.cpp Player.cpp Zombie.cpp Game.cpp -o zombie_game
```

**Run it:**
```bash
./zombie_game
```

On Windows with MinGW:
```bash
g++ main.cpp Player.cpp Zombie.cpp Game.cpp -o zombie_game.exe
zombie_game.exe
```

---

## Tips

- Zombies spawn faster over time - don't stand still!
- Use Q and E to turn and face the zombies before shooting
- The radar at the bottom shows how many zombies are close and how far
- You can type commands quickly if multiple zombies are swarming

---

## Known Issues / Things I'd Improve

- The game is turn-based not real-time - a proper game would use ncurses or SDL for real keyboard input
- Damage per frame is a bit weird since it's tied to turn speed
- No sound obviously
- The "3D" is really just X and Z coordinates, there's no actual rendering
- Could add walls/obstacles to make it more interesting
- Could add different zombie types with different speeds

---

*Made for CS intro course. This is my first C++ project after learning Python.*
