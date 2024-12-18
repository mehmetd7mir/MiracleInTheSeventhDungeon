#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "room.h"
#include "creature.h"
#include "item.h"
#include "combat.h"
#include "io.h"
#include <dirent.h>
#include <sys/stat.h>

// we read file content from path
static char *readFileContent(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("Failed to open file: %s\n", path); // Hangi dosya açılmadığını göster
        return NULL;
    }

    char buffer[300];
    size_t totalSize = 0;
    char *content = NULL;

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        size_t len = strlen(buffer);
        char *temp = realloc(content, totalSize + len + 1);
        if (!temp) {
            free(content);
            fclose(f);
            return NULL;
        }
        content = temp;
        strcpy(content + totalSize, buffer);
        totalSize += len;
    }

    fclose(f);
    return content;
}


// helper to create weapon
static Item* createWeapon(const char *name, int dmg, int bullet){
    Item *it = createItem(name, ITEM_WEAPON);
    it->data.weapon.damage = dmg;
    it->data.weapon.bulletCount = bullet;
    return it;
}

// helper to create heal
static Item* createHeal(const char *name, int strUp, float hpUp){
    Item *it = createItem(name, ITEM_HEAL);
    it->data.heal.strengthUp = strUp;
    it->data.heal.healthUpPercent = hpUp;
    return it;
}

// helper to create protect
static Item* createProtect(const char *name, float reduce){
    Item *it = createItem(name, ITEM_PROTECT);
    it->data.protect.damageReduce = reduce;
    it->data.protect.wearable = 1;
    return it;
}

// helper to create bullet
static Item* createBullet(const char *name, int qty){
    Item *it = createItem(name, ITEM_BULLET);
    it->data.bullet.quantity = qty;
    it->stackable = 1;
    return it;
}

// helper to create other
static Item* createOther(const char *name){
    Item *it = createItem(name, ITEM_OTHER);
    return it;
}

// find creature by name alive
static Creature* findCreature(Room *r, const char *name){
    if(!r || !name) return NULL;
    for(int i=0; i<r->creatureCount; i++){
        if(r->creatures[i]->isAlive && strcmp(r->creatures[i]->name, name) == 0)
            return r->creatures[i];
    }
    return NULL;
}

// check if can open box
static int canOpenBox(Room *r){
    if(!r) return 0;
    int haveBox = 0;
    int heatblastDead = 0;
    for(int i=0; i<r->itemCount; i++){
        if(strcmp(r->items[i]->name, "specialbox") == 0){
            haveBox = 1;
            break;
        }
    }
    for(int i=0; i<r->creatureCount; i++){
        if(strcmp(r->creatures[i]->name, "Heatblast") == 0 && r->creatures[i]->isAlive == 0){
            heatblastDead = 1;
            break;
        }
    }
    if(haveBox && heatblastDead) return 1;
    return 0;
}

// print menu
static void printMenu(){
    // menu show every step
    printf("\n----- MENU -----\n");
    printf("1- look\n");
    printf("2- pickup\n");
    printf("3- remove\n");
    printf("4- move\n");
    printf("5- attack\n");
    printf("6- inventory\n");
    printf("7- player info\n");
    printf("8- save\n");
    printf("9- exit\n");
    printf("10- load\n");
    printf("11- open box\n");
    printf("12- use item\n");
    printf("choose:\n");
}

// create all rooms with descriptions and items
Room **createAllRooms(int *count){
    *count = 8;
    
    
    // assuming descriptions are in ../description/
    // adjust path accordingly
    // but in main, we will use the correct path
    // here, just create rooms with placeholders

    // create directory path
    // but assume main handles it
    // read descriptions
    char descPath1[256], descPath2[256], descPath3[256], descPath4[256];
    char descPath5[256], descPath6[256], descPath7[256], descPathWin[256];
    snprintf(descPath1, sizeof(descPath1), "description/FirstRoomDescription.txt");
    snprintf(descPath2, sizeof(descPath2), "description/SecondRoomDescription.txt");
    snprintf(descPath3, sizeof(descPath3), "description/ThirdRoomDescription.txt");
    snprintf(descPath4, sizeof(descPath4), "description/FourthRoomDescription.txt");
    snprintf(descPath5, sizeof(descPath5), "description/FifthRoomDescription.txt");
    snprintf(descPath6, sizeof(descPath6), "description/SixthRoomDescription.txt");
    snprintf(descPath7, sizeof(descPath7), "description/SeventhRoomDescription.txt");
    snprintf(descPathWin, sizeof(descPathWin), "description/WrittenWhenWinGame.txt");

    char *d1 = readFileContent(descPath1);
    char *d2 = readFileContent(descPath2);
    char *d3 = readFileContent(descPath3);
    char *d4 = readFileContent(descPath4);
    char *d5 = readFileContent(descPath5);
    char *d6 = readFileContent(descPath6);
    char *d7 = readFileContent(descPath7);
    char *win = readFileContent(descPathWin);

    Room **arr = (Room**)malloc(sizeof(Room*)*8);
    arr[0] = createRoom(d1 ? d1 : "no desc");
    arr[1] = createRoom(d2 ? d2 : "no desc");
    arr[2] = createRoom(d3 ? d3 : "no desc");
    arr[3] = createRoom(d4 ? d4 : "no desc");
    arr[4] = createRoom(d5 ? d5 : "no desc");
    arr[5] = createRoom(d6 ? d6 : "no desc");
    arr[6] = createRoom(d7 ? d7 : "no desc");
    arr[7] = createRoom(win ? win : "no desc");

    if(d1) free(d1);
    if(d2) free(d2);
    if(d3) free(d3);
    if(d4) free(d4);
    if(d5) free(d5);
    if(d6) free(d6);
    if(d7) free(d7);
    if(win) free(win);

    // connect rooms
    arr[0]->north = arr[1];
    arr[1]->east = arr[2];
    arr[2]->west = arr[3];
    arr[3]->west = arr[4];
    arr[4]->east = arr[5];
    arr[5]->east = arr[6];
    arr[6]->north = arr[7];

    // creatures
    addCreatureToRoom(arr[1], createCreature("Blitzwolfer", 100.0f, 5.0f, 1.0f));
    addCreatureToRoom(arr[2], createCreature("Fourarms", 100.0f, 7.5f, 0.95f));
    addCreatureToRoom(arr[3], createCreature("Cannonbolt", 100.0f, 13.0f, 0.90f));
    addCreatureToRoom(arr[4], createCreature("Ghostfreak", 100.0f, 16.0f, 0.85f));
    addCreatureToRoom(arr[5], createCreature("Diomanhead", 100.0f, 22.0f, 0.80f));
    addCreatureToRoom(arr[6], createCreature("Heatblast", 100.0f, 25.0f, 0.75f));
    addCreatureToRoom(arr[6], createCreature("Vilgax", 100.0f, 30.0f, 0.60f));

    // add items to each room
    // room1 items
    addItemToRoom(arr[0], createWeapon("knife", 15, 0));
    addItemToRoom(arr[0], createWeapon("hammer", 20, 0));
    addItemToRoom(arr[0], createHeal("firstaidkit", 3, 0.25f));
    addItemToRoom(arr[0], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[0], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[0], createProtect("helmet1", 0.03f));

    // room2 items
    addItemToRoom(arr[1], createProtect("shield", 0.08f));
    addItemToRoom(arr[1], createWeapon("sword", 25, 0));
    addItemToRoom(arr[1], createProtect("armguard", 0.02f));
    addItemToRoom(arr[1], createProtect("kneepad", 0.02f));
    addItemToRoom(arr[1], createProtect("armor1", 0.08f));
    addItemToRoom(arr[1], createProtect("armoredboots2", 0.06f));
    addItemToRoom(arr[1], createHeal("energydrink", 1, 0.05f));
    addItemToRoom(arr[1], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[1], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[1], createHeal("injector", 0, 0.10f));
    addItemToRoom(arr[1], createHeal("firstaidkit", 3, 0.25f));

    // room3 items
    addItemToRoom(arr[2], createProtect("helmet2", 0.06f));
    addItemToRoom(arr[2], createProtect("armor2", 0.16f));
    addItemToRoom(arr[2], createHeal("energydrink", 1, 0.05f));
    addItemToRoom(arr[2], createHeal("energydrink", 1, 0.05f));
    addItemToRoom(arr[2], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[2], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[2], createHeal("injector", 0, 0.10f));
    addItemToRoom(arr[2], createHeal("injector", 0, 0.10f));
    addItemToRoom(arr[2], createWeapon("GlockG43", 35, 4));
    addItemToRoom(arr[2], createBullet("9mmbullet", 2));
    addItemToRoom(arr[2], createBullet("9mmbullet", 2));
    addItemToRoom(arr[2], createBullet("9mmbullet", 2));
    addItemToRoom(arr[2], createBullet("9mmbullet", 2));

    // room4 items
    addItemToRoom(arr[3], createHeal("injector", 0, 0.10f));
    addItemToRoom(arr[3], createHeal("firstaidkit", 3, 0.25f));
    addItemToRoom(arr[3], createWeapon("knife", 15, 0));
    addItemToRoom(arr[3], createBullet("9mmbullet", 2));
    addItemToRoom(arr[3], createBullet("9mmbullet", 2));
    addItemToRoom(arr[3], createProtect("armor1", 0.08f));
    addItemToRoom(arr[3], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[3], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[3], createWeapon("hammer", 20, 0));

    // room5 items
    addItemToRoom(arr[4], createHeal("injector", 0, 0.10f));
    addItemToRoom(arr[4], createHeal("injector", 0, 0.10f));
    addItemToRoom(arr[4], createHeal("firstaidkit", 3, 0.25f));
    addItemToRoom(arr[4], createWeapon("sword", 25, 0));
    addItemToRoom(arr[4], createProtect("armor2", 0.16f));
    addItemToRoom(arr[4], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[4], createWeapon("hammer", 20, 0));
    addItemToRoom(arr[4], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[4], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[4], createHeal("energydrink", 1, 0.05f));

    // room6 items
    addItemToRoom(arr[5], createHeal("firstaidkit", 3, 0.25f));
    addItemToRoom(arr[5], createWeapon("knife", 15, 0));
    addItemToRoom(arr[5], createProtect("armor1", 0.08f));
    addItemToRoom(arr[5], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[5], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[5], createWeapon("hammer", 20, 0));
    addItemToRoom(arr[5], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[5], createHeal("energydrink", 1, 0.05f));
    addItemToRoom(arr[5], createHeal("energydrink", 1, 0.05f));

    // room7 items
    addItemToRoom(arr[6], createHeal("firstaidkit", 3, 0.25f));
    addItemToRoom(arr[6], createHeal("firstaidkit", 3, 0.25f));
    addItemToRoom(arr[6], createWeapon("knife", 15, 0));
    addItemToRoom(arr[6], createProtect("armor2", 0.16f));
    addItemToRoom(arr[6], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[6], createHeal("bandage", 2, 0.10f));
    addItemToRoom(arr[6], createWeapon("hammer", 20, 0));
    addItemToRoom(arr[6], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[6], createHeal("painkiller", 0, 0.10f));
    addItemToRoom(arr[6], createHeal("energydrink", 1, 0.05f));
    addItemToRoom(arr[6], createHeal("energydrink", 1, 0.05f));
    addItemToRoom(arr[6], createOther("specialbox"));

    return arr;
}

// list saved games and return the number of saved games
// Remove this function from main.c since it's already in io.c
// int listSaveGames(const char *saveDir){
//     // This function is now handled in io.c
// }

int main(){
    // create directories if not exist
    mkdir("../save", 0777);
    mkdir("../description", 0777);

    int roomCount;
    Room **rooms = createAllRooms(&roomCount);
    
    // get player name
    char playerName[64];
    printf("enter your player name:\n");
    if(!fgets(playerName, sizeof(playerName), stdin)){
        printf("failed to read name.\n");
        return 1;
    }
    playerName[strcspn(playerName, "\n")] = 0;

    Player *player = createPlayer(rooms[0], playerName);

    int running = 1;
    while(running && player->lives > 0 && player->health > 0){
        // print room description only first time
        if(!player->currentRoom->visited){
            printf("%s\n", player->currentRoom->description);
            player->currentRoom->visited = 1;
        }
        printMenu();

        char line[256];
        if(!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;
        int choice = atoi(line);

        if(choice == 1){
            // look
            lookRoom(player->currentRoom);
        } else if(choice == 2){
            // pickup
            printf("enter item name to pickup:\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            line[strcspn(line, "\n")] = 0;
            Item *it = takeItemFromRoom(player->currentRoom, line);
            if(!it){
                printf("no such item in room!\n");
            } else {
                if(!playerAddItem(player, it)){
                    printf("cannot carry more items!\n");
                    addItemToRoom(player->currentRoom, it);
                } else {
                    printf("you took %s\n", it->name);
                }
            }
        } else if(choice == 3){
            // remove
            printf("enter item name to remove:\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            line[strcspn(line, "\n")] = 0;
            if(!playerRemoveItem(player, line, player->currentRoom)){
                printf("you don't have that item!\n");
            } else {
                printf("you dropped %s\n", line);
            }
        } else if(choice == 4){
            // move
            // check if any creature alive in room
            int hasAliveCreature = 0;
            for(int i=0; i<player->currentRoom->creatureCount; i++){
                if(player->currentRoom->creatures[i]->isAlive){
                    hasAliveCreature = 1;
                    break;
                }
            }
            if(hasAliveCreature){
                printf("you need to defeat all creatures before moving!\n");
                continue;
            }
            printf("enter direction (north,south,east,west):\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            line[strcspn(line, "\n")] = 0;
            Room *next = NULL;
            if(strcmp(line, "north") == 0) next = player->currentRoom->north;
            else if(strcmp(line, "south") == 0) next = player->currentRoom->south;
            else if(strcmp(line, "east") == 0) next = player->currentRoom->east;
            else if(strcmp(line, "west") == 0) next = player->currentRoom->west;
            if(!next){
                printf("can't go that way!\n");
            } else {
                player->currentRoom = next;
                // room description will be printed in next loop if not visited
                if(player->currentRoom == rooms[7]){
                    printf("%s\n", player->currentRoom->description);
                    running = 0;
                }
            }
        } else if(choice == 5){
            // attack
            printf("enter creature name to attack:\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            line[strcspn(line, "\n")] = 0;
            Creature *target = findCreature(player->currentRoom, line);
            if(!target){
                printf("no such alive creature here!\n");
            } else {
                startCombat(player, target);
            }
        } else if(choice == 6){
            // inventory
            printInventory(player);
        } else if(choice == 7){
            // player info
            printPlayerInfo(player);
        } else if(choice == 8){
            // save
            printf("enter save game name:\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            line[strcspn(line, "\n")] = 0;
            // construct save path
            char savePath[256];
            snprintf(savePath, sizeof(savePath), "../save/%s.save", line);
            if(saveGame(player, savePath)){
                printf("game saved as %s.\n", line);
            } else {
                printf("fail to save.\n");
            }
        } else if(choice == 9){
            // exit
            running = 0;
        } else if(choice == 10){
            // load
            printf("available saved games:\n");
            int numSaves = listSaveGames("../save");
            if(numSaves == 0){
                continue;
            }
            printf("enter the number of the save to load:\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            int saveNum = atoi(line);
            if(saveNum < 1 || saveNum > numSaves){
                printf("invalid save number.\n");
                continue;
            }
            // get the save file name
            DIR *d = opendir("../save");
            if(!d){
                printf("cannot open save directory.\n");
                continue;
            }
            struct dirent *dir;
            int current = 1;
            char selectedSave[256];
            while((dir = readdir(d)) != NULL){
                if(dir->d_type == DT_REG){
                    if(current == saveNum){
                        strcpy(selectedSave, dir->d_name);
                        break;
                    }
                    current++;
                }
            }
            closedir(d);
            // construct full path
            char fullPath[256];
            snprintf(fullPath, sizeof(fullPath), "../save/%s", selectedSave);
            if(loadGame(player, rooms, roomCount, fullPath)){
                printf("game loaded from %s.\n", selectedSave);
            } else {
                printf("fail to load.\n");
            }
        } else if(choice == 11){
            // open box
            if(canOpenBox(player->currentRoom)){
                Item *awm = createWeapon("AWM", 50, 2);
                addItemToRoom(player->currentRoom, awm);
                printf("specialbox opened and AWM added to the room!\n");
            } else {
                printf("cannot open box. maybe heatblast not dead or no box.\n");
            }
        } else if(choice == 12){
            // use item
            printf("enter item name to use:\n");
            if(!fgets(line, sizeof(line), stdin)) continue;
            line[strcspn(line, "\n")] = 0;
            if(playerUseItem(player, line)){
                // item used successfully
            } else {
                printf("failed to use item.\n");
            }
        } else {
            printf("unknown choice\n");
        }
    }

    // free all memory
    freePlayer(player);
    for(int i=0; i<roomCount; i++){
        freeRoom(rooms[i]);
    }
    free(rooms);

    return 0;
}

