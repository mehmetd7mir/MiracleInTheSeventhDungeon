#include "combat.h"
#include <stdio.h>
#include <string.h>
#include "room.h"
#include "item.h"

// helper function to calculate damage
// we choose weapon and calculate damage
static float playerAttackDamage(Player *p, const char *weaponName) {
    if(strcmp(weaponName, "punch") == 0) {
        return (float)p->strength;
    }
    for(int i=0; i<p->inventoryCount; i++){
        Item *it = p->inventory[i];
        if(it->type == ITEM_WEAPON && strcmp(it->name, weaponName) == 0){
            if(strstr(weaponName, "Glock") != NULL) {
                if(it->data.weapon.bulletCount <= 0){
                    printf("no bullet in this gun! turn wasted.\n");
                    return 0;
                }
                it->data.weapon.bulletCount -= 1;
                return (float)it->data.weapon.damage;
            } else if(strstr(weaponName, "AWM") != NULL){
                if(it->data.weapon.bulletCount <= 0){
                    printf("no bullet in awm! turn wasted.\n");
                    return 0;
                }
                it->data.weapon.bulletCount -= 1;
                return (float)it->data.weapon.damage;
            } else {
                return (float)it->data.weapon.damage;
            }
        }
    }
    printf("you don't have that weapon.\n");
    return 0;
}

// start combat
void startCombat(Player *p, Creature *c) {
    if(!p || !c || !c->isAlive) return;
    char creatureName[64];
    strcpy(creatureName, c->name);

    while(p->lives > 0 && p->health > 0 && c->isAlive){
        printf("choose your weapon (or 'punch'):\n");
        
        // list weapons
        for(int i=0; i<p->inventoryCount; i++){
            Item *it = p->inventory[i];
            if(it->type == ITEM_WEAPON){
                printf(" - %s (dmg:%d", it->name, it->data.weapon.damage);
                if(it->data.weapon.bulletCount > 0) {
                    printf(", bullet:%d", it->data.weapon.bulletCount);
                }
                printf(")\n");
            }
        }
        printf(" - punch(your strength:%d)\n", p->strength);

        char line[64];
        if(!fgets(line, 64, stdin)) return;
        line[strcspn(line, "\n")] = 0;

        float dmg = playerAttackDamage(p, line);
        if(dmg > 0){
            float before = c->health;
            creatureTakeDamage(c, dmg);
            float actual = before - c->health;
            printf("you used %s and dealt %.2f damage!\n", line, actual);
            if(!c->isAlive){
                printf("you defeated %s!\n", c->name);
                break;
            }
        } else {
            printf("no effective damage this turn.\n");
        }

        if(c->isAlive){
            printf("%s attacks you for %.2f damage!\n", c->name, c->attackPower);
            playerTakeDamage(p, c->attackPower);
            if(p->lives <= 0 || p->health <= 0){
                printf("you are dead!\n");
                break;
            }
        }
    }
    // no auto open box
}
