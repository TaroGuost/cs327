#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <endian.h>
#include <limits.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <ncurses.h>
#include <typeinfo>


#include "map.h"
#include "RLfile.h"
#include "pQueue.h"
#include "monster.h"
#include "path.h"


using namespace std;




bool Move( Charactor *M , int mc ,  Monster *Ori ,  point base[21][80], int px , int py, bool check);
bool MoveSix( Charactor *M, int px , int py ,  point base[21][80] , int mc , bool check ,  Monster *Ori);
bool Moverand( Charactor *M , point base[21][80] , int mc , bool check , int px ,int py , Monster *Ori);
bool IfSee(int x, int y , int px , int py , point base[21][80]);

void FakeValue(int px, int py , point base[21][80])
{
  int i,j;
  for(i = px-2 ; i < px+3 ; i++)
    {
      for(j = py -2; j < py+3 ; j++)
	{
	  if(base[i][j].ob != '.')
	    base[i][j].fake = base[i][j].ob;
	  else
	     base[i][j].fake = base[i][j].value;
		
	}
    }
  
}

bool IsAround(int x, int y, int px , int py)
{
  if(x>px +3 || x < px -3)
    return false;
  
  if(y>py+3 ||  y < py-3)
    return false;


  return true;

}


void Minsert(MonNode** head , Charactor *M)
{
  MonNode* temp = (*head);

  MonNode* New = NewMNode(M);

  if(*head == NULL)
    {
      New->next = *head;
      (*head) = New;
    }
  else
    {
      if((((*head)->M)->s) > M->s)
	{
	  New->next = *head;
	  (*head) = New;	
	}
      else
	{
	  while(temp->next !=NULL && (temp->next->M)->s <
		(New->M)->s)
	    {
	      temp = temp->next;
	    }

	  New->next = temp->next;
	  temp->next = New;
	}
    }

}

Charactor* Mpop(MonNode **head )
{
  Charactor *M = (*head)->M;
  MonNode* temp = (*head);
  (*head) = (*head)->next;
  free(temp);

  return M;
}




int GeneStair( point base[21][80])
{
  int i,j;
  
  for(i = 0 ; i < 21 ; i ++)
    {
      for(j = 0 ; j < 80 ; j++)
	{
	  if(base[i][j].value == '.')
	    {
	      base[i][j].value = '<';
	      return i*80+j;
	    }
	} 
    }

  return 0;
  
}

int GeneDStair( point base[21][80])
{
  int i,j;
  
  for(i = 19 ; i >= 0 ; i --)
    {
      for(j = 79 ; j >= 0 ; j--)
	{
	  if(base[i][j].value == '.')
	    {
	      base[i][j].value = '>';
	      return i*80+j;
	    }
	} 
    }

  return 0;
}

int isSamePlace( Monster *Monsters , int mc , int x , int y)
{

  int i;
  for(i = 0 ; i < mc ; i ++ )
    {
      if((Monsters+i)->x == x && (Monsters+i)->y == y && (Monsters+i)->Dead == false)
	{
	  return i;
	}
    }
  return -1;
}

void PrintMonster(WINDOW **game , Monster *ms , int mc ,  point base[21][80] , int px , int py , int stair , int Dstair,bool check , vector<objects> ob)
{
  int i,j;

  for(i = 0 ; i < 21 ; i ++)
    {
      for(j = 0 ; j < 80 ; j ++)
	{
	  if(base[i][j].value == ' ' && base[i][j].hardness == 0)
	    {
	      base[i][j].value = '#';
	    }	

	  if(i == stair/80 && j == stair%80)
	    {
	       base[i][j].value ='<';	     
	    }

	   if(i == Dstair/80 && j == Dstair%80)
	    {
	       base[i][j].value ='>';	     
	    }

	       FakeValue(px,py,base);
	   

	  int temp = isSamePlace(ms,mc,i,j);
	   if(!check)
	    {
	      if(temp  != -1 && IsAround(i,j,px,py) )
		{
		  int n = (*(ms+temp)).usefulColor();

		  init_pair(n, n, COLOR_BLACK);
		  wattron(*game,COLOR_PAIR(n));
		  mvwprintw(*game,i,j,"%c", (ms+temp)->Sybol);
		  wattroff(*game,COLOR_PAIR(n));
		}
	      else
		{		  
		  if(base[i][j].ob != '.')
		    {
		      int n = ob.at(base[i][j].position).SelectColor();
		      init_pair(n, n, COLOR_BLACK);
		      wattron(*game,COLOR_PAIR(n));
		      mvwprintw(*game,i,j,"%c" , base[i][j].fake);
		      wattroff(*game,COLOR_PAIR(n));
		    }
		  else
		    mvwprintw(*game,i,j,"%c" , base[i][j].fake);
	 
		}
	    }
	  else
	    {
	      if(temp  != -1)
		{
		  int n = (*(ms+temp)).usefulColor();

		  init_pair(n, n, COLOR_BLACK);
		  wattron(*game,COLOR_PAIR(n));
		  mvwprintw(*game,i,j,"%c", (ms+temp)->Sybol);
		  wattroff(*game,COLOR_PAIR(n));
		}
	      else
		{
		  if(base[i][j].ob != '.')
		    {
		      int n = ob.at(base[i][j].position).SelectColor();
		      init_pair(n, n, COLOR_BLACK);
		      wattron(*game,COLOR_PAIR(n));
		      mvwprintw(*game,i,j,"%c" , base[i][j].ob);
		      wattroff(*game,COLOR_PAIR(n));
		    }
		  else
		    mvwprintw(*game,i,j,"%c" , base[i][j].value);
		  
		  refresh();
		}
	    }
	  
	  if(i == px && j == py)
	    {
	      mvwprintw(*game,i,j,"%c" , '@');
	    }
  
	}
    }

}

bool EMove( Charactor *Monsters, Monster *Ori , int mc , int px , int py , point base[21][80] , char temp)
{
  bool dead = false;
  
  switch(temp)
    {
    case 0 :
      dead = Moverand(Monsters,base, mc, false , px , py , Ori);
      break;
    case 1:
      if(Monsters->x == px || Monsters->y == py)
	dead = Move(Monsters,mc,Ori,base,px,py,false);
      else
	dead = Moverand(Monsters,base, mc, false , px , py , Ori);
      break;
    case 2:
      dead = MoveSix(Monsters,px ,py ,base , mc , false, Ori);
      break;
    case 3:
      dead = Move(Monsters,mc,Ori,base,px,py,false);
      break;
    case 4:
      dead = Moverand(Monsters,base,mc,true,px,py,Ori);
      break;
    case 5:
      if(Monsters->x == px || Monsters->y == py)
	dead = Move(Monsters,mc,Ori,base,px,py,true);
      else
	dead = Moverand(Monsters,base, mc, true , px , py , Ori);
      break;
    case 6:
      dead = MoveSix(Monsters , px , py , base ,mc ,true,Ori);
      break;
    case 7:
      dead = Move(Monsters,mc,Ori,base,px,py,true);
      break;
    }

  return dead;

}

void PrintLose(Charactor *Monsters)
{
  int i, j;
  for(i = 0 ; i < 21 ; i ++)
    {
      for(j = 0 ; j < 80 ; j ++)
	{
	  if(i == 21/2 && j == 80/2)
	    {
	      printf("YOU DEAD!!!!   ");
	      printf("%c KILLED YOU", ((Monster*)Monsters)->Sybol);
	    }
	  else
	    {
	      printf(" ");
	    }
	}
      printf("\n");
    }


}



void PrintList(WINDOW **list,int mc, int px ,int py, Monster *M,int n)
{
  int i;

  int x = 5;

  if(x > mc)
    x =mc;
  init_pair(1, COLOR_RED, COLOR_BLACK);
  wattron(*list, COLOR_PAIR(1));
  for(i = 0  ; i<x ; i++)
    {
      int Rx = px - (M+i+n)->x;
      int Ry = py - (M+i+n)->y;
	  
      if(Rx > 0 && Ry>0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d North and %d West ",(M+i+n)->type , Rx , Ry);
	}
      else if(Rx > 0 && Ry < 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d North and %d East ",(M+i+n)->type , Rx , (Ry*-1));
	}
      else if(Rx < 0 && Ry > 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d South and %d West ",(M+i+n)->type , Rx*-1 , Ry);
	}
      else if(Rx < 0 && Ry < 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d South and %d East ",(M+i+n)->type , Rx*-1 , Ry*-1);
	}
      else if(Rx > 0 && Ry == 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d North",(M+i+n)->type , Rx);
	}
      else if(Rx < 0 && Ry == 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d South",(M+i+n)->type , Rx*-1);
	}
      else if(Rx == 0 && Ry < 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d West ",(M+i+n)->type , Ry*-1);
	}
      else if(Rx == 0 && Ry > 0)
	{
	  mvwprintw(*list,i+1,1,"%lx, %d East ",(M+i+n)->type , Rx*-1);
	}
       
    }

   wattroff(*list,COLOR_PAIR(1));

}

void MonsterList( Monster *M, int mc , int px , int py)
{
  int i = 0;
  int n = 0;

  WINDOW *list;
  list = newwin(8,30,10,20);
  keypad(list,true);
  wborder(list,'*','*','*','*','*','*','*','*');
  while(i != 1)
    {
      PrintList(&list,mc,px,py,M,n);
      int ch = wgetch(list);
      switch(ch)
	{
	case 27:
	  i =1;
	  break;
	case 'e':
	  i =1;
	  break;
	case KEY_DOWN:
	  if(n+5<mc)
	    n++;
	  break;
	case KEY_UP:
	  if(n>0)
	    n--;
	  break;
	}
      
    }
  

  //werase(list);
  wclrtoeol(list);
  wrefresh(list);
  delwin(list);
  //endwin();
  
  
}

void killed(Monster*Ori , Player p , int mc)
{
  for(int i = 0 ; i < mc ; i++)
    {
      if((Ori+i)->x == p.x && (Ori+i)->y == p.y && (Ori+i)->Dead != true)
	(Ori+i)->Dead = true;
    }
}


void PCteleport( WINDOW **game, point base[21][80] , Player* M , bool fog)
{

  int ch;
  int x =10;
  int y = 40;
  char temp = base[x][y].fake;
  bool check = false;
    
  
  mvwprintw(*game, x , y ,"%c" , '*' );

  while(!check)
    {
      ch = wgetch(*game);
      
      switch(ch)
	{
	case KEY_UP:
	  if(x-1 > 0)
	    {
	      mvwprintw(*game, x , y , "%c" , temp);
	      x--;
	      if(fog)
		temp = base[x][y].fake;
	      else
		temp = base[x][y].value;
	      mvwprintw(*game, x , y, "%c" , '*');
	    }
	  break;
	case KEY_DOWN:
	  if(x+1 <=19)
	    {
	      mvwprintw(*game, x , y , "%c" , temp);
	      x++;
	      if(fog)
		temp = base[x][y].fake;
	      else
		temp = base[x][y].value;
		  mvwprintw(*game, x , y, "%c" , '*');
	    }
	  break;
	case KEY_LEFT:
	  if(y-1 >0)
	    {
	      mvwprintw(*game, x , y , "%c" , temp);
	      y--;
	      if(fog)
		temp = base[x][y].fake;
	      else
		temp = base[x][y].value;
		  mvwprintw(*game, x , y, "%c" , '*');
	    }
	  break;
	case KEY_RIGHT:
	  if(y+1 <=78)
	    {
	      mvwprintw(*game, x , y , "%c" , temp);
	      y++;
	      if(fog)
		temp = base[x][y].fake;
	      else
		temp = base[x][y].value;
	      mvwprintw(*game, x , y, "%c" , '*');
	    }
	  break;
	case 'g':
	  check = true;
	  break;
	case 'r':
	  mvwprintw(*game, x , y , "%c" , temp);
	  x = (rand()%19)+1;
	  y = (rand()%78)+1;
	  check = true;
	  break;
	}
      
    }

  M->x = x;
  M->y = y;

}


char PCMove( Charactor *M ,  point base[21][80], WINDOW **game , char store,  Monster *Ori,int mc,int stair, int Dstair , vector<objects> *ob)
{

  base[M->x][M->y].value = store;
  base[M->x][M->y].fake = store;
  bool check = true;
  bool visite = false;
   
    
   while(check == true)
  {
       
      int ch = wgetch(*game);
      switch(ch)
	{
	case 'j':
	  if(base[(M->x)+1][M->y].hardness ==0)
	    {
	      M->x = (M->x)+1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'k':
	  if(base[(M->x)-1][M->y].hardness ==0)
	    {
	      M->x = (M->x)-1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'h':
	  if(base[M->x][(M->y)-1].hardness ==0)
	    {
	      M->y = (M->y)-1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'l':
	  if(base[M->x][(M->y)+1].hardness ==0)
	    {
	      M->y = (M->y)+1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'y':
	  if(base[(M->x)-1][(M->y)-1].hardness ==0)
	    {
	      M->y = (M->y)-1;
	      M->x = (M->x)-1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'u':
	  if(base[(M->x)-1][(M->y)+1].hardness ==0)
	    {
	      M->y = (M->y)+1;
	      M->x = (M->x)-1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'b':
	  if(base[(M->x)+1][(M->y)-1].hardness ==0)
	    {
	      M->y = (M->y)-1;
	      M->x = (M->x)+1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 'n':
	  if(base[(M->x)+1][(M->y)+1].hardness ==0)
	    {
	      M->y = (M->y)+1;
	      M->x = (M->x)+1;
	      store =  base[M->x][M->y].value;
	      base[M->x][M->y].value = '@';
	      check = false;
	    }
	  break;
	case 32:
	  check =false;
	  store =  base[M->x][M->y].value;
	  break;
	case 'Q':
	  check = false;
	  store = '!';
	  break;
	case 'f':
	  visite = !visite;
	  PrintMonster(game,Ori,mc,base,M->x,M->y,stair,Dstair,visite, *ob);
	  break;
	case 'g':
	  PCteleport(game,base,(Player*)M, !visite);
	  store = base[M->x][M->y].value;
	  base[M->x][M->y].value = '@';
	  check = false;
	  break;
	case 'm':
	  MonsterList(Ori,mc,M->x,M->y);
	  PrintMonster(game, Ori , mc,base,M->x,M->y,stair,Dstair,visite,*ob);
	  break;
	case '<':
	  if(M->x == stair/80 && M->y == stair%80)
	    {
	      store = '^';
	      check = false;
	    }
	case '>':
	  if(M->x == Dstair/80 && M->y == Dstair%80)
	    {
	      store = '^';
	      check = false;
	    }
	  break;
	      
	}
      }

 	
  return store;

  
}


bool MonsterMove( Monster *Monsters , int mc,  point base[21][80] , int px , int py , vector<objects> *ob)
{
  Player *player = (Player*)malloc(sizeof(Player));
  addPlayer(player , px ,py);
  int i;
  bool dead = false;
  MonNode* mq = NewMNode(player);
  bool change = false;
  bool seeP = false;
  char store = '.';
  const int stair = GeneStair(base);
  const int Dstair = GeneDStair(base);

  
  for(i = 0 ; i < mc ; i++)
    {
      Minsert(&mq , Monsters+i);
    }
  
  initscr();
  noecho();
  cbreak();
  start_color();
  
  WINDOW *game;
  game = newwin(24,80,0,0);

  keypad(game,true);
   
     
  while(!dead)
    {
      Charactor*  M = Mpop(&mq);

	  if(M->Dead == true)
	    {
	      continue;
	    }
      
      switch(M->type)
	{
	case 0:
	  dead = Moverand(M,base, mc, false , px , py , Monsters);
	  break;
	case 1:
	  if((M)->x == px || (M)->y == py)
	    dead = Move(M,mc,Monsters,base,px,py,false);
	  else
	    dead = Moverand(M,base, mc, false , px , py , Monsters);
	  break;
	case 2:
	  dead = MoveSix(M,px ,py ,base , mc , false,Monsters);
	  break;
	case 3:
	  dead = Move(M,mc,Monsters,base,px,py,false);
	  break;
	case 4:
	  dead = Moverand(M,base,mc,true,px,py,Monsters);
	  break;
	case 5:
	  if(M->x == px || M->y == py || change ==true)
	    {
	      dead = Move(M,mc,Monsters,base,px,py,true);
	      change = true;
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, true , px , py , Monsters);
	    }
	  break;
	case 6:
	  dead = MoveSix(M , px , py , base ,mc ,true,Monsters);
	  break;
	case 7:
	  dead = Move(M,mc,Monsters,base,px,py,true);
	  break;
	case 8:
	  dead = Moverand(M,base, mc, false , px , py , Monsters);
	  break;
	case 9:
	  if(rand()%2 == 0)
	    {
	      if(M->x == px || M->y == py || seeP ==true)
		{
		  dead = Move(M,mc,Monsters,base,px,py,false);
		  seeP = true;
		}
	      else
		{
		  dead = Moverand(M,base, mc, false , px , py , Monsters);
		}
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, false , px , py , Monsters);
	    }
	  break;
	case 10:
	  if(rand()%2 ==0)
	    {
	      dead = MoveSix(M,px ,py ,base , mc , false , Monsters);
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, false , px , py , Monsters);
	    }
	  break;
	case 11:
	  dead = EMove(M,Monsters, mc , px , py,base, Mtype(1,1,2,2));
	  break;
	case 12:
	  if(rand()%2 ==0)
	    {
	      dead = Moverand(M,base,mc,true,px,py,Monsters);
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, false , px , py , Monsters);
	    }
	  break;
	case 13:
	  dead =  EMove(M,Monsters, mc , px , py,base, Mtype(1,2,1,2));
	  break;
	case 14:
	  dead =  EMove(M,Monsters, mc , px , py,base , Mtype(1,2,2,1));
	  break;
	case 15:
	  dead =  EMove(M,Monsters, mc , px , py,base, Mtype(1,2,2,2));
	  break;
	case '@':
	  PrintMonster(&game,Monsters, mc , base,px,py , stair , Dstair,false , *ob);
	  store = PCMove(M,base,&game,store,Monsters,mc,stair,Dstair,ob);
	  killed(Monsters,*player,mc);
	  px = M->x;
	  py = M->y;
	  break;
	}

      if(store == '!')
	{
	  clrtoeol();
	  refresh();
	  endwin();
	  printf("\n YourQuitGame!!!D:\n");
	  break;
	}

      if(store == '^')
	{
	  clrtoeol();
	  refresh();
	  endwin();
	  return true;
	}
	 
      if(dead)
	{
	  clrtoeol();
	  refresh();
	  endwin();
	  PrintLose(M);
	  break;  
	}

	  
      (M)->s += 10;
      Minsert(&mq , M);
      //mq = NewMNode(player);
    }

  return false;
       
}


bool Move( Charactor *M, int mc ,  Monster *Ori ,  point base[21][80],int px, int py, bool check)
{
  int movex[8] = {-80,-80,-80,0,0,80,80,80};
  int movey[8] = {-1,0,1,-1,1,-1,0,1};
  int i;
  int x = 0;
  int y = 0;
  int dis[1680];

  if(check == true)
    FindShortestPathWithWall(base , &base[px][py],dis);
  else
    FindShortestPath(base,&base[px][py],dis);
  

  int min = dis[calculateindex(M->x,M->y)];
   

   
  for(i = 0 ; i < 8 ; i ++)
    {

      int v = calculateindex(M->x,M->y) + movex[i] + movey[i];

      if( v<=0 || v>=1680)
	{
	  continue;
	}
     
      if(dis[v] != -1 )
	{
        
	  if(dis[v] < min  && isSamePlace(Ori , mc , v/80 , v%80) == -1)
	    {	
	      x = v/80;
	      y = v%80;
	      min = dis[v];
	    }

	  
	}

    }

  if(M->x == px && M->y == py)
    {
      return true;
    }
  
  if(dis[calculateindex(M->x,M->y)]- dis[calculateindex(x,y)] > 1  )
    {
      dis[calculateindex(x,y)]++;
    }
  else
    {

      if(x != 0 && y !=0)
	{
	  (M->x)= x;
	  (M->y)= y;
	  base[x][y].hardness = 0;
        }

      if(M->x == px && M->y == py)
	{
	  return true;
	}

    }
  
  return false;
     
}

bool MoveSix( Charactor *M, int px , int py ,  point base[21][80], int mc , bool check,  Monster *Ori)
{
  int referx = px - M->x;
  int refery = py - M->y;

  if((referx == 0 && refery ==1) || (referx == 0 && refery ==-1))
    {
      return true;
    }

  
  if(referx != 0)
    {
      if(referx > 0)
	{
	  
	  if(base[(M->x)+1][M->y].value != ' ' && isSamePlace(Ori , mc , (M->x)+1 , (M->y)) == -1 && check == false)
	    {
	      M->x = (M->x)+1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if(isSamePlace(Ori , mc , (M->x)+1 , (M->y)) == -1 && check == true)
	    {
	      if(base[(M->x)+1][M->y].hardness/85 > 0)
		{
		  base[(M->x)+1][M->y].hardness -= 85;
		}
	      else
		{
		  M->x = (M->x)+1;
		  base[M->x][M->y].hardness = 0;
		}
	    }
	}
      else if(referx < 0)
	{
	  if(base[(M->x)-1][M->y].value != ' ' && isSamePlace(Ori , mc , (M->x)-1 , (M->y)) == -1 && check == false)
	    {
	      M->x = (M->x)-1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if(isSamePlace(Ori , mc , (M->x)-1 , (M->y)) == -1 && check == true)
	    {
	      if(base[(M->x)-1][M->y].hardness/85 > 0)
		{
		  base[(M->x)-1][M->y].hardness -= 85;
		}
	      else
		{
		  M->x = (M->x)-1;
		  base[M->x][M->y].hardness = 0;
		}
	    }

	}
    }
  else if(refery != 0 || (referx == (px - M->x) && referx != 0))
    {

      if(refery > 0)
	{
	  if(base[M->x][(M->y)+1].value != ' ' && isSamePlace(Ori , mc , (M->x) , (M->y)+1) == -1 && check == false)
	    {
	      M->y = (M->y) +1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if(isSamePlace(Ori , mc , (M->x) , (M->y)+1) == -1 && check == true)
	    {
	      if(base[M->x][(M->y)+1].hardness/85 > 0)
		{
		  base[M->x][(M->y)+1].hardness -=85;
		}
	      else
		{
		  M->y = (M->y)+1;
		  base[M->x][M->y].hardness = 0;
		}
	    }
	}
      else if(refery <0)
	{
	  if(base[M->x][(M->y)-1].value != ' ' && isSamePlace(Ori , mc , (M->x) , (M->y)-1) == -1 && check == false)
	    {
	      M->y = (M->y) -1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if(isSamePlace(Ori , mc , (M->x) , (M->y)-1) == -1 && check == true)
	    {
	      if(base[M->x][(M->y)-1].hardness/85 > 0)
		{
		  base[M->x][(M->y)-1].hardness -=85;
		}
	      else
		{
		  M->y = (M->y)-1;
		  base[M->x][M->y].hardness = 0;
		}
	    }
	}

    }
  else
    {
      return true;
    }

  return false;

}

bool Moverand( Charactor *M , point base[21][80] , int mc , bool check , int px ,int py, Monster *Ori)
{
  int movex[8] = {-1,-1,-1,0,0,1,1,1};
  int movey[8] = {-1,0,1,-1,1,-1,0,1};
  int i = rand()%8;

  int  x = (M->x)+movex[i];
  int  y = (M->y)+movey[i];
  if(x > 19 || x < 1  || y >78 || y <1 )
    {
      return false;
    }
   
  if(check == false)
    {

      if(base[x][y].hardness == 0 && isSamePlace(Ori , mc , x, y) == -1)
	{
	  M->x = x;
	  M->y = y;
	}
    }
  else
    {
      if( isSamePlace(Ori , mc , x, y) == -1)
	{
	  if(base[x][y].hardness/85 > 0)
	    {
	      base[x][y].hardness -= 85;
	    }
	  else
	    {
	      M->x = x;
	      M->y = y;
	      base[M->x][M->y].hardness = 0;
	    }
	}
    }
   
  if(M->x == px && M->y == py)
    {
      return true;
    }

  return false;
}

void cleanObject(point base[21][80] , vector<objects> *ob)
{
  
  for(int i = 0 ; i < 21 ; i++)
    {
      for(int j = 0 ; j < 80 ; j ++)
	{
	  if(base[i][j].ob != '.')
	    {
	      base[i][j].ob = '.';
	      base[i][j].position = -1;
	    }
	}
    }

  (*ob).clear();
}

int main (int argc , char *argv[])
{
  int i,n;
  point Base[21][80];
  srand (time(NULL));
  int size = 0;
  int Msize;
  int MonsterCount = 10;
  room *rooms;
  int playlocation[2];
  bool issave = false;
  bool isload = false;
  Monster *Monsters;
  Monster *Cala = (Monster*)malloc(sizeof(Monster));
  vector<objects> ob;
  vector<objects> obj;
  
  rooms = ( room*)malloc(10*sizeof( room));

  char *home = getenv("HOME");
  char *dir = strcat(home , "/.rlg327");
  //dir = strcat(dir, "/dungeon");
  mkdir(dir,0777);  

  string s = dir;
  string filename = s + "/monster_desc.txt";
  string fname = s + "/object_desc.txt";
  
  n = argc;

  for( i = 0 ; i < n ; i++)
    {
      if(strcmp("--save" , argv[i])==0)
	{
	  issave =true;
	}
      else if(strcmp("--load" , argv[i])==0)
	{
	  isload = true;
	}
      if(strcmp("--n" , argv[i]) == 0)
	{
	  if(argv[i+1] != NULL)
	    {
	      MonsterCount = atoi(argv[i+1]);
	    }
	}
    }

  Monsters = ( Monster*)malloc((MonsterCount)*sizeof( Monster));
	
  if(isload)
    {
      loadFile(Base , rooms , &size , dir , playlocation );
    }
  else
    {
      generatemap(Base , &size , rooms , playlocation);	  
    }
		
  if(issave)
    {
      WriteFile(size , playlocation , Base , rooms , dir);
    }

  Msize = ImportMonster(&Cala,filename);
  tempc(Cala , Msize);
  GMonster(rooms , Monsters , size ,Base , MonsterCount , Cala , Msize);
  ImportObject(&ob,fname);
  Gobject(&obj,rooms,size,Base , ob);
  printobject(&ob);
  
  //PrintMonster(Monsters , MonsterCount, Base,playlocation[0],playlocation[1]);	   
  bool check = MonsterMove(Monsters , MonsterCount, Base , playlocation[0] , playlocation[1], &obj);

  while(check)
    {
      cleanObject(Base , &obj);
      generatemap(Base , &size , rooms , playlocation);
      GMonster(rooms , Monsters , size ,Base , MonsterCount , Cala , Msize);
      Gobject(&obj,rooms,size,Base , ob);
      check = MonsterMove(Monsters , MonsterCount, Base , playlocation[0] , playlocation[1],&obj);
    }

  free(Cala);
  return 0;

}

