#include "player.h"

// create player
// player carry no special bag now
Player *createPlayer(Room *startRoom, const char *name){
    Player *p = (Player*)malloc(sizeof(Player));
    if(!p) return NULL;
    p->name = (char*)malloc(strlen(name)+1);
    if(!p->name){ free(p); return NULL; }
    strcpy(p->name, name);
    p->health = 100.0f;
    p->maxHealth = 100.0f;
    p->lives = 2;
    p->strength = 3;
    p->inventory = NULL;
    p->inventoryCount = 0;
    p->inventoryCapacity = 25;
    p->protectItems = NULL;
    p->protectCount = 0;
    p->currentRoom = startRoom;
    return p;
}

// free player memory
void freePlayer(Player *p){
    if(!p) return;
    if(p->name) free(p->name);
    if(p->inventory){
        for(int i=0; i<p->inventoryCount; i++){
            if(p->inventory[i]) freeItem(p->inventory[i]);
        }
        free(p->inventory);
    }
    if(p->protectItems){
        for(int i=0; i<p->protectCount; i++){
            if(p->protectItems[i]) freeItem(p->protectItems[i]);
        }
        free(p->protectItems);
    }
    free(p);
}

// check if can add item
int playerCanAddItem(Player *p, Item *it){
    if(!p || !it) return 0;
    if(p->inventoryCount >= p->inventoryCapacity) return 0;
    return 1;
}

// add item to inventory
int playerAddItem(Player *p, Item *it){
    if(!p || !it) return 0;
    if(!playerCanAddItem(p, it)){
        return 0;
    }
    // check if item is stackable and already in inventory
    if(it->stackable){
        for(int i=0; i<p->inventoryCount; i++){
            if(p->inventory[i] && strcmp(p->inventory[i]->name, it->name) == 0){
                p->inventory[i]->data.bullet.quantity += it->data.bullet.quantity;
                freeItem(it);
                return 1;
            }
        }
    }
    // add new item
    Item **newArr = (Item**)realloc(p->inventory, sizeof(Item*)*(p->inventoryCount+1));
    if(!newArr) return 0;
    p->inventory = newArr;
    p->inventory[p->inventoryCount] = it;
    p->inventoryCount++;
    return 1;
}

// take item from inventory
Item *playerTakeItem(Player *p, const char *name){
    if(!p || !name) return NULL;
    for(int i=0; i<p->inventoryCount; i++){
        if(p->inventory[i] && strcmp(p->inventory[i]->name, name) == 0){
            Item *taken = p->inventory[i];
            // if stackable and quantity >1, reduce quantity
            if(taken->stackable && taken->data.bullet.quantity > 1){
                taken->data.bullet.quantity -= 1;
                return copyItem(taken);
            }
            // else remove the item from array
            for(int j=i; j<p->inventoryCount-1; j++){
                p->inventory[j] = p->inventory[j+1];
            }
            p->inventoryCount--;
            if(p->inventoryCount == 0){
                free(p->inventory);
                p->inventory = NULL;
            } else {
                Item **newArr = (Item**)realloc(p->inventory, sizeof(Item*)*p->inventoryCount);
                if(newArr) p->inventory = newArr;
            }
            return taken;
        }
    }
    return NULL;
}

// wear protect item
int playerWearProtectItem(Player *p, Item *it){
    if(!p || !it) return 0;
    if(it->type != ITEM_PROTECT) return 0;
    Item **newArr = (Item**)realloc(p->protectItems, sizeof(Item*)*(p->protectCount+1));
    if(!newArr) return 0;
    p->protectItems = newArr;
    p->protectItems[p->protectCount] = it;
    p->protectCount++;
    return 1;
}

// total protect reduce
float playerTotalProtectReduce(Player *p){
    if(!p) return 0.0f;
    float total = 0.0f;
    for(int i=0; i<p->protectCount; i++){
        total += p->protectItems[i]->data.protect.damageReduce;
    }
    return total;
}

// player take damage
void playerTakeDamage(Player *p, float dmg){
    if(!p) return;
    float reduce = playerTotalProtectReduce(p);
    float realDmg = dmg * (1.0f - reduce);
    if(realDmg < 0) realDmg = 0;
    p->health -= realDmg;
    if(p->health <= 0){
        p->lives -= 1;
        if(p->lives > 0){
            p->health = p->maxHealth;
        } else {
            p->health = 0.0f;
        }
    }
}

// use heal item
void playerUseHealItem(Player *p, Item *healItem){
    if(!p || !healItem) return;
    if(healItem->type != ITEM_HEAL) return;
    p->strength += healItem->data.heal.strengthUp;
    float newHealth = p->health + p->maxHealth * healItem->data.heal.healthUpPercent;
    if(newHealth > p->maxHealth) newHealth = p->maxHealth;
    p->health = newHealth;
}

// use any usable item
int playerUseItem(Player *p, const char *name){
    if(!p || !name) return 0;
    for(int i=0; i<p->inventoryCount; i++){
        if(p->inventory[i] && strcmp(p->inventory[i]->name, name) == 0){
            Item *item = p->inventory[i];
            if(item->type == ITEM_HEAL){
                playerUseHealItem(p, item);
                // remove item after use
                if(item->stackable){
                    if(item->data.bullet.quantity > 1){
                        item->data.bullet.quantity -= 1;
                    } else {
                        // remove from inventory
                        for(int j=i; j<p->inventoryCount-1; j++){
                            p->inventory[j] = p->inventory[j+1];
                        }
                        p->inventoryCount--;
                        if(p->inventoryCount == 0){
                            free(p->inventory);
                            p->inventory = NULL;
                        } else {
                            Item **newArr = (Item**)realloc(p->inventory, sizeof(Item*)*p->inventoryCount);
                            if(newArr) p->inventory = newArr;
                        }
                        freeItem(item);
                    }
                } else {
                    // non-stackable, remove item
                    for(int j=i; j<p->inventoryCount-1; j++){
                        p->inventory[j] = p->inventory[j+1];
                    }
                    p->inventoryCount--;
                    if(p->inventoryCount == 0){
                        free(p->inventory);
                        p->inventory = NULL;
                    } else {
                        Item **newArr = (Item**)realloc(p->inventory, sizeof(Item*)*p->inventoryCount);
                        if(newArr) p->inventory = newArr;
                    }
                    freeItem(item);
                }
                printf("you used %s\n", name);
                return 1;
            }
            // can add other item types here
        }
    }
    printf("you don't have that usable item!\n");
    return 0;
}

// print inventory
void printInventory(Player *p){
    if(!p) return;
    printf("player inventory(%d/%d):\n", p->inventoryCount, p->inventoryCapacity);
    // count items with same name
    for(int i=0; i<p->inventoryCount; i++){
        if(p->inventory[i]){
            int count = 1;
            for(int j=i+1; j<p->inventoryCount; j++){
                if(p->inventory[j] && strcmp(p->inventory[i]->name, p->inventory[j]->name) == 0){
                    count++;
                    // remove duplicate
                    freeItem(p->inventory[j]);
                    for(int k=j; k<p->inventoryCount-1; k++){
                        p->inventory[k] = p->inventory[k+1];
                    }
                    p->inventoryCount--;
                    j--;
                }
            }
            printf("  %s x%d\n", p->inventory[i]->name, count);
        }
    }
}

// print player info
void printPlayerInfo(Player *p){
    if(!p) return;
    printf("player health:%.2f/%.2f\n", p->health, p->maxHealth);
    printf("player lives:%d\n", p->lives);
    printf("player strength:%d\n", p->strength);
}

// remove item from inventory and add to room
int playerRemoveItem(Player *p, const char *name, Room *r){
    if(!p || !r || !name) return 0;
    for(int i=0; i<p->inventoryCount; i++){
        if(p->inventory[i] && strcmp(p->inventory[i]->name, name) == 0){
            Item *removed = p->inventory[i];
            // if stackable and quantity >1, reduce quantity
            if(removed->stackable && removed->data.bullet.quantity > 1){
                removed->data.bullet.quantity -= 1;
                return 1;
            }
            // else remove the item from array
            for(int j=i; j<p->inventoryCount-1; j++){
                p->inventory[j] = p->inventory[j+1];
            }
            p->inventoryCount--;
            if(p->inventoryCount == 0){
                free(p->inventory);
                p->inventory = NULL;
            } else {
                Item **newArr = (Item**)realloc(p->inventory, sizeof(Item*)*p->inventoryCount);
                if(newArr) p->inventory = newArr;
            }
            // add to room
            addItemToRoom(r, removed);
            return 1;
        }
    }
    return 0;
}
