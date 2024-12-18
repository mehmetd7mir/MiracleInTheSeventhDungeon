#include "item.h"

// create item memory
// we create item, not complicated
Item *createItem(const char *name, ItemType type) {
    Item *it = (Item*)malloc(sizeof(Item));
    if(!it) return NULL;
    it->name = (char*)malloc(strlen(name)+1);
    if(!it->name) { free(it); return NULL; }
    strcpy(it->name, name);
    it->type = type;
    it->stackable = 0;
    it->unique = 0;
    switch(type){
        case ITEM_WEAPON:
            it->data.weapon.damage = 0;
            it->data.weapon.bulletCount = 0;
            break;
        case ITEM_BULLET:
            it->data.bullet.quantity = 1;
            it->stackable = 1;
            break;
        case ITEM_HEAL:
            it->data.heal.strengthUp = 0;
            it->data.heal.healthUpPercent = 0.0f;
            break;
        case ITEM_PROTECT:
            it->data.protect.damageReduce = 0.0f;
            it->data.protect.wearable = 1;
            break;
        case ITEM_OTHER:
            it->data.other.quantity = 1;
            break;
    }
    return it;
}

// free item memory
void freeItem(Item *item){
    if(!item) return;
    if(item->name) free(item->name);
    free(item);
}

// copy item
// simple copy all fields
Item *copyItem(const Item *s){
    if(!s) return NULL;
    Item *it = createItem(s->name, s->type);
    if(!it) return NULL;
    it->stackable = s->stackable;
    it->unique = s->unique;
    switch(s->type){
        case ITEM_WEAPON:
            it->data.weapon.damage = s->data.weapon.damage;
            it->data.weapon.bulletCount = s->data.weapon.bulletCount;
            break;
        case ITEM_BULLET:
            it->data.bullet.quantity = s->data.bullet.quantity;
            break;
        case ITEM_HEAL:
            it->data.heal.strengthUp = s->data.heal.strengthUp;
            it->data.heal.healthUpPercent = s->data.heal.healthUpPercent;
            break;
        case ITEM_PROTECT:
            it->data.protect.damageReduce = s->data.protect.damageReduce;
            it->data.protect.wearable = s->data.protect.wearable;
            break;
        case ITEM_OTHER:
            it->data.other.quantity = s->data.other.quantity;
            break;
    }
    return it;
}

// print item
void printItem(const Item *item){
    if(!item) return;
    printf("item: %s type:%d\n", item->name, item->type);
}
