#ifndef PLAYER_H
#define PLAYER_H

#include "room.h"

// player struct
typedef struct {
    char *name; // player name
    float health;
    float maxHealth;
    int lives;
    int strength;
    Item **inventory;
    int inventoryCount;
    int inventoryCapacity; // fixed at 25
    Item **protectItems;
    int protectCount;
    Room *currentRoom;
} Player;

// create player
Player *createPlayer(Room *startRoom, const char *name);
// free player memory
void freePlayer(Player *p);
// add item to player inventory
int playerAddItem(Player *p, Item *it);
// take item from inventory
Item *playerTakeItem(Player *p, const char *name);
// wear protect item
int playerWearProtectItem(Player *p, Item *it);
// total protect reduce
float playerTotalProtectReduce(Player *p);
// player take damage
void playerTakeDamage(Player *p, float dmg);
// use heal item
void playerUseHealItem(Player *p, Item *healItem);
// use any usable item
int playerUseItem(Player *p, const char *name);
// print inventory
void printInventory(Player *p);
// print player info
void printPlayerInfo(Player *p);
// remove item from inventory and add to room
int playerRemoveItem(Player *p, const char *name, Room *r);
// check if can add item
int playerCanAddItem(Player *p, Item *it);

#endif
