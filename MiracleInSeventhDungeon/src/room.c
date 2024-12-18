#include "room.h"

// create room
Room *createRoom(const char *desc){
    Room *r = (Room*)malloc(sizeof(Room));
    if(!r) return NULL;
    r->description = (char*)malloc(strlen(desc)+1);
    if(!r->description){ free(r); return NULL; }
    strcpy(r->description, desc);
    r->items = NULL;
    r->itemCount = 0;
    r->creatures = NULL;
    r->creatureCount = 0;
    r->north = NULL;
    r->south = NULL;
    r->east = NULL;
    r->west = NULL;
    r->visited = 0;
    return r;
}

// free room memory
void freeRoom(Room *r){
    if(!r) return;
    if(r->items){
        for(int i=0; i<r->itemCount; i++){
            if(r->items[i]) freeItem(r->items[i]);
        }
        free(r->items);
    }
    if(r->creatures){
        for(int i=0; i<r->creatureCount; i++){
            if(r->creatures[i]) freeCreature(r->creatures[i]);
        }
        free(r->creatures);
    }
    if(r->description) free(r->description);
    free(r);
}

// add item to room
int addItemToRoom(Room *r, Item *it){
    if(!r || !it) return 0;
    // check if item is stackable and already exists
    if(it->stackable){
        for(int i=0; i<r->itemCount; i++){
            if(r->items[i] && strcmp(r->items[i]->name, it->name) == 0){
                r->items[i]->data.bullet.quantity += it->data.bullet.quantity;
                freeItem(it);
                return 1;
            }
        }
    }
    // add new item
    Item **newArr = (Item**)realloc(r->items, sizeof(Item*)*(r->itemCount+1));
    if(!newArr) return 0;
    r->items = newArr;
    r->items[r->itemCount] = it;
    r->itemCount++;
    return 1;
}

// add creature to room
int addCreatureToRoom(Room *r, Creature *c){
    if(!r || !c) return 0;
    Creature **newArr = (Creature**)realloc(r->creatures, sizeof(Creature*)*(r->creatureCount+1));
    if(!newArr) return 0;
    r->creatures = newArr;
    r->creatures[r->creatureCount] = c;
    r->creatureCount++;
    return 1;
}

// find item in room
Item *findItemInRoom(Room *r, const char *name){
    if(!r || !name) return NULL;
    for(int i=0; i<r->itemCount; i++){
        if(r->items[i] && strcmp(r->items[i]->name, name) == 0)
            return r->items[i];
    }
    return NULL;
}

// take item from room
Item *takeItemFromRoom(Room *r, const char *name){
    if(!r || !name) return NULL;
    for(int i=0; i<r->itemCount; i++){
        if(r->items[i] && strcmp(r->items[i]->name, name) == 0){
            Item *taken = r->items[i];
            // if stackable and quantity >1, reduce quantity
            if(taken->stackable && taken->data.bullet.quantity > 1){
                taken->data.bullet.quantity -= 1;
                return copyItem(taken);
            }
            // else remove the item from array
            for(int j=i; j<r->itemCount-1; j++){
                r->items[j] = r->items[j+1];
            }
            r->itemCount--;
            if(r->itemCount == 0){
                free(r->items);
                r->items = NULL;
            } else {
                Item **newArr = (Item**)realloc(r->items, sizeof(Item*)*r->itemCount);
                if(newArr) r->items = newArr;
            }
            return taken;
        }
    }
    return NULL;
}

// print room items and creatures
void lookRoom(Room *r){
    if(!r){
        printf("no room\n");
        return;
    }
    printf("items in room(%d):\n", r->itemCount);
    // count items with same name
    for(int i=0; i<r->itemCount; i++){
        if(r->items[i]){
            int count = 1;
            for(int j=i+1; j<r->itemCount; j++){
                if(r->items[j] && strcmp(r->items[i]->name, r->items[j]->name) == 0){
                    count++;
                    // remove duplicate
                    freeItem(r->items[j]);
                    for(int k=j; k<r->itemCount-1; k++){
                        r->items[k] = r->items[k+1];
                    }
                    r->itemCount--;
                    j--;
                }
            }
            printf("  %s x%d\n", r->items[i]->name, count);
        }
    }
    printf("creatures in room(%d):\n", r->creatureCount);
    for(int i=0; i<r->creatureCount; i++){
        if(r->creatures[i]->isAlive)
            printf("  %s\n", r->creatures[i]->name);
    }
}
