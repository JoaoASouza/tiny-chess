#ifndef RENDERER_H
#define RENDERER_H

#include "engine.h"
#include <stdio.h>

char* pieceToChar(int piece);
void clearScreen();
void printBoard(GameState state, char message[100]);

#endif