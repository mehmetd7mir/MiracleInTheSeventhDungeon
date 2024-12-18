#include "io.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>

// save game basic
// we do only basic save
int saveGame(Player *p, const char *filepath){
    FILE *f = fopen(filepath, "w");
    if(!f) return 0;
    fprintf(f, "%s\n", p->name);
    fprintf(f, "%.2f %.2f %d %d\n", p->health, p->maxHealth, p->lives, p->strength);
    fprintf(f, "%d %d\n", p->inventoryCount, p->inventoryCapacity);
    for(int i=0; i<p->inventoryCount; i++){
        fprintf(f, "%s %d\n", p->inventory[i]->name, p->inventory[i]->type);
    }
    // protect items
    fprintf(f, "%d\n", p->protectCount);
    for(int i=0; i<p->protectCount; i++){
        fprintf(f, "%s %.2f\n", p->protectItems[i]->name, p->protectItems[i]->data.protect.damageReduce);
    }
    fclose(f);
    return 1;
}

// load game basic
int loadGame(Player *p, Room **rooms, int roomCount, const char *filepath){
    (void)rooms;
    (void)roomCount;
    FILE *f = fopen(filepath, "r");
    if(!f) return 0;
    char name[64];
    if(fgets(name, sizeof(name), f) == NULL){ fclose(f); return 0; }
    name[strcspn(name, "\n")] = 0;
    // free old name and set new name
    if(p->name){
        free(p->name);
    }
    p->name = (char*)malloc(strlen(name)+1);
    if(!p->name){ fclose(f); return 0; }
    strcpy(p->name, name);
    
    float health, maxh; int lives, str;
    if(fscanf(f, "%f %f %d %d", &health, &maxh, &lives, &str) != 4){ fclose(f); return 0; }
    p->health = health;
    p->maxHealth = maxh;
    p->lives = lives;
    p->strength = str;
    int invCount, invCap;
    if(fscanf(f, "%d %d", &invCount, &invCap) != 2){ fclose(f); return 0; }
    // clear old inventory
    for(int i=0; i<p->inventoryCount; i++){
        freeItem(p->inventory[i]);
    }
    free(p->inventory);
    p->inventory = NULL;
    p->inventoryCount = 0;
    p->inventoryCapacity = invCap > 0 ? invCap : 25;

    for(int i=0; i<invCount; i++){
        char iname[64]; int t;
        if(fscanf(f, "%s %d", iname, &t) != 2){ fclose(f); return 0; }
        Item *it = createItem(iname, (ItemType)t);
        playerAddItem(p, it);
    }

    // load protect items
    int protCount;
    if(fscanf(f, "%d", &protCount) != 1){ fclose(f); return 0; }
    for(int i=0; i<protCount; i++){
        char pname[64]; float reduce;
        if(fscanf(f, "%s %f", pname, &reduce) != 2){ fclose(f); return 0; }
        Item *prot = createItem(pname, ITEM_PROTECT);
        prot->data.protect.damageReduce = reduce;
        playerWearProtectItem(p, prot);
    }

    fclose(f);
    return 1;
}

// list saved games
int listSaveGames(const char *saveDir){
    DIR *d;
    struct dirent *dir;
    d = opendir(saveDir);
    if(d){
        printf("saved games:\n");
        int index = 1;
        while((dir = readdir(d)) != NULL){
            if(dir->d_type == DT_REG){ // regular file
                printf("  %d- %s\n", index, dir->d_name);
                index++;
            }
        }
        closedir(d);
        if(index == 1){
            printf("  no saved games found.\n");
            return 0;
        }
        return index - 1;
    }
    printf("cannot open save directory.\n");
    return 0;
}


