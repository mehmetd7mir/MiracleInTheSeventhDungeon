# Miracle In Seventh Dungeon

## Overview

**Miracle In Seventh Dungeon** is a text-based adventure game developed in C. Players navigate through seven distinct rooms, battling formidable creatures, collecting valuable items, and managing their inventory to survive and conquer the dungeon. The game features a robust save and load system, allowing players to preserve their progress and continue their adventure at any time.

## Gameplay

### Starting the Game
Upon launching the game, players are prompted to enter their character's name. This name personalizes the gaming experience and is saved alongside the game state.

### Exploring Rooms
- **Look (`1`)**: Examine the current room to view available items and any present creatures.
- **Pickup (`2`)**: Collect items from the room by specifying the item's name.
- **Remove (`3`)**: Drop items from your inventory into the current room.
- **Move (`4`)**: Navigate to adjacent rooms by specifying a direction (north, south, east, west). Note that you must defeat all creatures in the current room before moving.

### Combat
- **Attack (`5`)**: Engage in combat with creatures by specifying the creature's name. Choose your weapon or opt to punch to deal damage. Combat continues in turns until either the player or the creature is defeated.

### Inventory Management
- **Inventory (`6`)**: View all items currently in your possession.
- **Player Info (`7`)**: Check your character's health, lives, and strength.
- **Use Item (`12`)**: Utilize consumable items like health kits or painkillers to restore health or boost strength.

### Saving and Loading
- **Save (`8`)**: Save your current game state by providing a unique save name. All save files are stored in the `save/` directory.
- **Load (`10`)**: Load a previously saved game from the `save/` directory by selecting the desired save file.

### Special Actions
- **Open Box (`11`)**: Open special boxes found within rooms to obtain rare items, provided certain conditions are met (e.g., defeating specific creatures).

### Exiting the Game
- **Exit (`9`)**: Quit the game at any time.

## Code Structure

The project is organized into several components to ensure modularity and ease of maintenance:

- **src/**: Contains all source code files.
  - `main.c`: Handles the game loop, user input, and overall game flow.
  - `item.c` & `item.h`: Define item structures and related functions.
  - `creature.c` & `creature.h`: Define creature structures and combat mechanics.
  - `room.c` & `room.h`: Manage room creation, connections, and room-specific actions.
  - `player.c` & `player.h`: Manage player attributes, inventory, and player-related actions.
  - `combat.c` & `combat.h`: Handle combat interactions between the player and creatures.
  - `commands.c` & `commands.h`: (Currently unused) Intended for future command implementations.
  - `io.c` & `io.h`: Manage saving and loading game states.
- **save/**: Directory where all saved game files (`.save`) are stored.
- **description/**: Contains room description files (`.rtf`) that provide narrative details for each room.
- **Makefile**: Automates the build process, compiling source files and linking them into an executable.
- **README.md**: Provides an overview and instructions for the project.
- **.gitignore**: Specifies files and directories to be ignored by Git, such as compiled objects and save files.

## Game Logic

Players traverse through interconnected rooms, each with its unique description and challenges. The primary objectives and mechanics include:

- **Room Navigation**: Rooms are linked via directional paths (north, south, east, west). Each room may contain items to collect and creatures to battle.
  
- **Combat System**: Players can engage creatures using various weapons or fists. Each creature has distinct health and attack power. Successful combat requires strategic use of items and weapons to defeat enemies.
  
- **Inventory Management**: Players collect items such as weapons, healing kits, and protective gear. Managing inventory effectively is crucial for survival and progression.
  
- **Save and Load Mechanism**: Players can save their progress at any point and load from saved states to continue their adventure. This feature ensures that progress is not lost and allows players to experiment with different strategies.

- **Special Items**: Certain rooms contain special boxes that can be opened under specific conditions, granting access to rare and powerful items.

## Installation and Compilation

### Prerequisites
- **GCC Compiler**: Ensure that GCC is installed on your system. You can verify this by running:
  ```bash
  gcc --version
