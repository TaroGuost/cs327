# ifndef MONSTER_H
# define MONSTER_H

#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<climits>
#include"map.h"

class Charactor
{
public:
  int x;
  int y;
  int speed;
  int s;
  char type;
  bool Dead;
};

class Monster: public Charactor
{
};

class Player : public Charactor
{
  
};

void addPlayer(Player *p , int px , int py);
void GMonster( room *rooms ,  Monster *Monsters , int size ,  point base[21][80] , int mc);
char Mtype(int a, int b , int c, int d);
bool IfPC( room *r ,  point base[21][80]);


#endif
