# ifndef MONSTER_H
# define MONSTER_H

#include<iostream>
#include<string>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<climits>
#include<fstream>
#include<iomanip>
#include<vector>
#include<iterator>
#include"map.h"

using namespace std;

enum Ability{SMART = 1, TELE =10 ,TUNNEL=100, ERRATIC = 1000 , NONE =0 ,PASS = -1 ,PICKUP =-2, DESTORY = -3 , UNIQ = -4 , BOSS = -5 };

class dices
{
public:
  int side=0;
  int Amount=0;
  int base=0;
public:
  void setup(vector<string> s);
  int Roll();
  string toString();
};


class Charactor
{
public:
  int x;
  int y;
  dices speed;
  int s;
  char type;
  bool Dead;
};

class Monster: public Charactor
{
public:
  Ability a[9] = {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE}; 
  string name;
  dices HP;
  dices Damage;
  int RRTY = 12;
  string Color;
  char Sybol;
  string Desc;

  void setType();
  string displayAbil();
};

class Player : public Charactor
{
  
};

void addPlayer(Player *p , int px , int py);
void GMonster( room *rooms ,  Monster *Monsters , int size ,  point base[21][80] , int mc);
char Mtype(int a, int b , int c, int d);
bool IfPC( room *r ,  point base[21][80]);
void setCount(string s ,int *count);
int ImportMonster(Monster **M , string Filename);
void tempc(Monster *M , int size);
vector<string> Split(string s , char c);


#endif
