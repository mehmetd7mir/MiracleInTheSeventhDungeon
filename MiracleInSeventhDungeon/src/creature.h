#ifndef CREATURE_H
#define CREATURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// creature struct
typedef struct {
    char *name;
    float health;
    float attackPower;
    float damageFactor; 
    int isAlive;
} Creature;

// create creature
Creature *createCreature(const char *name, float health, float attackPower, float damageFactor);
// free creature memory
void freeCreature(Creature *c);
// copy creature
Creature *copyCreature(const Creature *source);
// print creature
void printCreature(const Creature *c);
// creature take damage
void creatureTakeDamage(Creature *c, float dmg);

#endif
