#ifndef MAP_H
# define MAP_H

#include <stdio.h>
#include <math.h>

class point
{
public:
  int x;
  int y;
  bool check;
  char fake;
  char value;
  char ob = '.';
  int hardness;
  int position = -1;
};

class room
{
public:
  int x;
  int y;
  int rw;
  int rl;
};


void initialBase ( point b[21][80]);

int CreatRoom ( point p[21][80],  room *r);

int CreatRoom ( point p[21][80],  room *r);

void playerlocate( room *r , int size ,  point p[21][80], int loco[2]);

void makeCodder( point p[21][80] , int FromX, int FromY, int ToX, int ToY);

int build ( point p[21][80], int x, int y, int width, int length,room* r);

void generatemap( point Base[21][80] , int *size ,  room *rooms , int playlocation[2]);



#endif
