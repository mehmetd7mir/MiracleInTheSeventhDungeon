#ifndef ROOM_H
#define ROOM_H

#include "item.h"
#include "creature.h"

// room struct
typedef struct Room {
    char *description;
    Item **items;
    int itemCount;
    Creature **creatures;
    int creatureCount;
    struct Room *north;
    struct Room *south;
    struct Room *east;
    struct Room *west;
    int visited; // to check if room was visited
} Room;

// create room
Room *createRoom(const char *desc);
// free room memory
void freeRoom(Room *r);
// add item to room
int addItemToRoom(Room *r, Item *it);
// add creature to room
int addCreatureToRoom(Room *r, Creature *c);
// find item in room
Item *findItemInRoom(Room *r, const char *name);
// take item from room
Item *takeItemFromRoom(Room *r, const char *name);
// print room items and creatures
void lookRoom(Room *r);

#endif
