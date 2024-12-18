#include "creature.h"

// create creature
Creature *createCreature(const char *name, float health, float attackPower, float damageFactor){
    Creature *c = (Creature*)malloc(sizeof(Creature));
    if(!c) return NULL;
    c->name = (char*)malloc(strlen(name)+1);
    if(!c->name) { free(c); return NULL; }
    strcpy(c->name, name);
    c->health = health;
    c->attackPower = attackPower;
    c->damageFactor = damageFactor;
    c->isAlive = 1;
    return c;
}

// free creature memory
void freeCreature(Creature *c){
    if(!c) return;
    if(c->name) free(c->name);
    free(c);
}

// copy creature
Creature *copyCreature(const Creature *s){
    if(!s) return NULL;
    Creature *c = createCreature(s->name, s->health, s->attackPower, s->damageFactor);
    if(c) c->isAlive = s->isAlive;
    return c;
}

// print creature
void printCreature(const Creature *c){
    if(!c) return;
    printf("creature %s hp:%.2f att:%.2f df:%.2f alive:%d\n", c->name, c->health, c->attackPower, c->damageFactor, c->isAlive);
}

// creature take damage
void creatureTakeDamage(Creature *c, float dmg){
    if(!c || !c->isAlive) return;
    float realDamage = dmg * c->damageFactor;
    c->health -= realDamage;
    if(c->health <= 0){
        c->health = 0;
        c->isAlive = 0;
    }
}
