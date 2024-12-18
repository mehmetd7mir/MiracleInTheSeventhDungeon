#ifndef IO_H
#define IO_H

#include "player.h"

// save game
int saveGame(Player *p, const char *filepath);
// load game
int loadGame(Player *p, Room **rooms, int roomCount, const char *filepath);
// list saved games
int listSaveGames(const char *saveDir);

#endif
