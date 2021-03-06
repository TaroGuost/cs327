#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include "pQueue.h"
#include "map.h"
#include "monster.h"
#include "print.h"
#include "path.h"

bool Move( Charactor *M , int mc ,  Monster *Ori ,  point base[21][80], int px , int py, bool check,Player *p,string *atr);
bool MoveSix( Charactor *M, int px , int py ,  point base[21][80] , int mc , bool check ,  Monster *Ori,Player *p,string *atr);
bool Moverand( Charactor *M , point base[21][80] , int mc , bool check , int px ,int py , Monster *Ori,Player *p , string *atr);
bool EMove(Charactor *Monsters , Monster *Ori , int mc , int px ,int py , point base[21][80] , char temp , Player *p , string *atr);
bool IfSee(int x, int y , int px , int py , point base[21][80]);
int isSamePlace(Monster *Monsters , int mc , int x , int y);
bool killed(WINDOW **game ,Monster *Ori , int mc , int px ,int py , Player *p , char *s);
char PCMove(Charactor *M, point base[21][80],WINDOW **game , char store, Monster *Ori ,int mc , int stair , int Dstair , vector<objects> *ob);
bool MonsterMove(Monster *Monsters , int mc , point base[21][80] , int px ,int py, vector<objects> *ob , Player *player);
void swapMonster(Monster *Ori , Monster *M , int x , int y , int mc);
bool IsAround(int x, int y , int px ,int py);

#endif
