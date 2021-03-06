#ifndef PATH_H
#define PATH_H

#include "map.h"
#include "pQueue.h"
#include "monster.h"
#include "move.h"

void printPath(int dis[1680] , Node **head , point *source);
int calculateindex(int x , int y);
int getlength(int p);
void FindShortestPath(point base[21][80] , point *source , int dis[1680]);
void printWallPath(int dis[1680],point *source);
void FindShortestPathWithWall(point base[21][80] , point *source , int dis[1680]);

#endif
