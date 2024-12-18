#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this enum for item type. we keep it simple
typedef enum {
    ITEM_WEAPON,
    ITEM_BULLET,
    ITEM_HEAL,
    ITEM_PROTECT,
    ITEM_OTHER
} ItemType;

// weapon properties
typedef struct {
    int damage;
    int bulletCount;
} WeaponProps;

// bullet properties
typedef struct {
    int quantity;
} BulletProps;

// heal properties
typedef struct {
    int strengthUp;
    float healthUpPercent;
} HealProps;

// protect properties
typedef struct {
    float damageReduce;
    int wearable;
} ProtectProps;

// other properties
typedef struct {
    int quantity;
} OtherProps;

// item struct
typedef struct {
    char *name;
    ItemType type;
    int stackable;
    int unique;
    union {
        WeaponProps weapon;
        BulletProps bullet;
        HealProps heal;
        ProtectProps protect;
        OtherProps other;
    } data;
} Item;

// function to create item
Item *createItem(const char *name, ItemType type);
// free item memory
void freeItem(Item *item);
// copy item
Item *copyItem(const Item *source);
// print item
void printItem(const Item *item);

#endif
