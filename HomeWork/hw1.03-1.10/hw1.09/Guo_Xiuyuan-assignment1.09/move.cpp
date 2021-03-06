#include "move.h"

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

bool EMove( Charactor *Monsters, Monster *Ori , int mc , int px , int py , point base[21][80] , char temp, Player *p , string *atr)
{
  bool dead = false;
  
  switch(temp)
    {
    case 0 :
      dead = Moverand(Monsters,base, mc, false , px , py , Ori,p,atr);
      break;
    case 1:
      if(Monsters->x == px || Monsters->y == py)
	dead = Move(Monsters,mc,Ori,base,px,py,false,p,atr);
      else
	dead = Moverand(Monsters,base, mc, false , px , py , Ori,p,atr);
      break;
    case 2:
      dead = MoveSix(Monsters,px ,py ,base , mc , false, Ori,p,atr);
      break;
    case 3:
      dead = Move(Monsters,mc,Ori,base,px,py,false,p,atr);
      break;
    case 4:
      dead = Moverand(Monsters,base,mc,true,px,py,Ori,p,atr);
      break;
    case 5:
      if(Monsters->x == px || Monsters->y == py)
	dead = Move(Monsters,mc,Ori,base,px,py,true,p,atr);
      else
	dead = Moverand(Monsters,base, mc, true , px , py , Ori,p,atr);
      break;
    case 6:
      dead = MoveSix(Monsters , px , py , base ,mc ,true,Ori,p,atr);
      break;
    case 7:
      dead = Move(Monsters,mc,Ori,base,px,py,true,p,atr);
      break;
    }

  return dead;

}

bool killed(WINDOW **game,Monster*Ori , int mc , int px ,int py , Player *p , char *s)
{
  int n = p->Damage.Roll();
  
  for(int i = 0 ; i < mc ; i++)
    {
      if((Ori+i)->x == px && (Ori+i)->y == py && (Ori+i)->Dead == false)
	{
	  if((Ori+i)->HP <= n)
	    {
	      if((Ori+i)->isBoss)
		{
		  mvwprintw(*game,23,5,"You have killed BOSSSSSSS : %c" , (Ori+i)->Sybol);
		  *s = '+';
		  return false;
		}
	      
	      (Ori+i)->Dead = true;
	      mvwprintw(*game,23,5,"                                               ");
	      mvwprintw(*game,23,5,"You have killed Monster : %c" , (Ori+i)->Sybol);
	      
	      return true;
	    }
	  else
	    {
	      (Ori+i)->HP -= n;
	      mvwprintw(*game,23,5,"                                               ");
	      mvwprintw(*game,23,5,"%c's HP : %d",(Ori+i)->Sybol,(Ori+i)->HP);
	      move(23,0);
	      clrtoeol();
	      
	      return false;
	    }
	}
    }
  return true;
}

char PCMove( Charactor *M ,  point base[21][80], WINDOW **game , char store,  Monster *Ori,int mc,int stair, int Dstair , vector<objects> *ob)
{

  base[M->x][M->y].value = store;
  base[M->x][M->y].fake = store;
  bool check = true;
  bool visite = false;
  
  if(base[M->x][M->y].ob != '.')
    {
      if(((Player*)M)->PickupItem(ob , base[M->x][M->y].position, base))
	{
	  base[M->x][M->y].ob = '.';
	  base[M->x][M->y].position = -1;
	}
    }
  
   while(check == true)
  {
       
      int ch = wgetch(*game);
      switch(ch)
	{
	case 'j':
	  if(base[(M->x)+1][M->y].hardness ==0)
	    {
	      if(killed(game , Ori , mc , (M->x)+1, M->y ,(Player*) M,&store))
	      {
		  M->x = (M->x)+1;
		  store =  base[M->x][M->y].value;
		  base[M->x][M->y].value = '@';
	      }
	      check = false;
	    }
	  break;
	case 'k':
	  if(base[(M->x)-1][M->y].hardness ==0)
	    {
	      if(killed(game , Ori , mc , (M->x)-1, M->y ,(Player*) M,&store))
	      {
		  M->x = (M->x)-1;
		  store =  base[M->x][M->y].value;
		  base[M->x][M->y].value = '@';
	      }
	      check = false;
	    }
	  break;
	case 'h':
	  if(base[M->x][(M->y)-1].hardness ==0)
	    {
	      if(killed(game , Ori , mc , (M->x), (M->y)-1 , (Player*)M,&store))
		{
		  M->y = (M->y)-1;
		  store =  base[M->x][M->y].value;
		  base[M->x][M->y].value = '@';
		}
	      check = false;
	    }
	  break;
	case 'l':
	  if(base[M->x][(M->y)+1].hardness ==0)
	    {
	      if(killed(game , Ori , mc , (M->x), (M->y)+1 , (Player*)M , &store))
		{
		  M->y = (M->y)+1;
		  store =  base[M->x][M->y].value;
		  base[M->x][M->y].value = '@';
		}
	      check = false;
	    }
	  break;
	case 'y':
	  if(base[(M->x)-1][(M->y)-1].hardness ==0)
	    {
	      if(killed(game , Ori , mc , (M->x)-1, (M->y)-1 , (Player*)M,&store))
		{
		  M->y = (M->y)-1;
		  M->x = (M->x)-1;
		  store =  base[M->x][M->y].value;
		  base[M->x][M->y].value = '@';
		}
	      check = false;
	    }
	  break;
	case 'u':
	  if(base[(M->x)-1][(M->y)+1].hardness ==0)
	    {
	      if(killed(game , Ori , mc , (M->x)+1, (M->y)-1 , (Player*)M,&store))
		{
		  M->y = (M->y)+1;
		  M->x = (M->x)-1;
		  store =  base[M->x][M->y].value;
		  base[M->x][M->y].value = '@';
		}
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
	  PrintMonster(game,Ori,mc,base,M->x,M->y,stair,Dstair,visite, *ob,(Player*)M);
	  break;
	case 'g':
	  PCteleport(game,base,(Player*)M, !visite);
	  store = base[M->x][M->y].value;
	  base[M->x][M->y].value = '@';
	  check = false;
	  break;
	case 'm':
	  MonsterList(Ori,mc,M->x,M->y);
	  PrintMonster(game, Ori , mc,base,M->x,M->y,stair,Dstair,visite,*ob,(Player*)M);
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
	case 'i':
	  slotList((Player*)M , base , ob);
	  PrintMonster(game, Ori , mc,base,M->x,M->y,stair,Dstair,visite,*ob,(Player*)M);
	  break;
	case 'e':
	  equiList((Player*)M);
	  PrintMonster(game, Ori , mc,base,M->x,M->y,stair,Dstair,visite,*ob,(Player*)M);
	  break;
	case 'L':
	  CheckMonster(game,Ori,base,mc ,!visite);
	  PrintMonster(game, Ori, mc, base , M->x, M->y , stair,Dstair , visite, *ob , (Player*)M);
	  break;
	}
      }

 	
  return store;

  
}


bool MonsterMove( Monster *Monsters , int mc,  point base[21][80] , int px , int py , vector<objects> *ob , Player *player)
{
  addPlayer(player , px ,py);
  int i;
  bool dead = false;
  MonNode* mq = NewMNode(player);
  bool change = false;
  bool seeP = false;
  char store = '.';
  const int stair = GeneStair(base);
  const int Dstair = GeneDStair(base);
  string atr = "";
  
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
	  dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
	  break;
	case 1:
	  if((M)->x == px || (M)->y == py)
	    dead = Move(M,mc,Monsters,base,px,py,false,player,&atr);
	  else
	    dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
	  break;
	case 2:
	  dead = MoveSix(M,px ,py ,base , mc , false,Monsters,player,&atr);
	  break;
	case 3:
	  dead = Move(M,mc,Monsters,base,px,py,false,player,&atr);
	  break;
	case 4:
	  dead = Moverand(M,base,mc,true,px,py,Monsters,player,&atr);
	  break;
	case 5:
	  if(M->x == px || M->y == py || change ==true)
	    {
	      dead = Move(M,mc,Monsters,base,px,py,true,player,&atr);
	      change = true;
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, true , px , py , Monsters,player,&atr);
	    }
	  break;
	case 6:
	  dead = MoveSix(M , px , py , base ,mc ,true,Monsters,player,&atr);
	  break;
	case 7:
	  dead = Move(M,mc,Monsters,base,px,py,true,player,&atr);
	  break;
	case 8:
	  dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
	  break;
	case 9:
	  if(rand()%2 == 0)
	    {
	      if(M->x == px || M->y == py || seeP ==true)
		{
		  dead = Move(M,mc,Monsters,base,px,py,false,player,&atr);
		  seeP = true;
		}
	      else
		{
		  dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
		}
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
	    }
	  break;
	case 10:
	  if(rand()%2 ==0)
	    {
	      dead = MoveSix(M,px ,py ,base , mc , false , Monsters,player,&atr);
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
	    }
	  break;
	case 11:
	  dead = EMove(M,Monsters, mc , px , py,base, Mtype(1,1,2,2),player,&atr);
	  break;
	case 12:
	  if(rand()%2 ==0)
	    {
	      dead = Moverand(M,base,mc,true,px,py,Monsters,player,&atr);
	    }
	  else
	    {
	      dead = Moverand(M,base, mc, false , px , py , Monsters,player,&atr);
	    }
	  break;
	case 13:
	  dead =  EMove(M,Monsters, mc , px , py,base, Mtype(1,2,1,2),player,&atr);
	  break;
	case 14:
	  dead =  EMove(M,Monsters, mc , px , py,base , Mtype(1,2,2,1),player,&atr);
	  break;
	case 15:
	  dead =  EMove(M,Monsters, mc , px , py,base, Mtype(1,2,2,2),player,&atr);
	  break;
	case '@':
	  PrintMonster(&game,Monsters, mc , base,px,py , stair , Dstair,false , *ob , (Player*)M);
	  if(atr.compare("") != 0)
	    {
	      atr += "attacked you!";
	      mvwprintw(game, 22 , 40 , "%s" , atr.c_str());
	    }
	  store = PCMove(M,base,&game,store,Monsters,mc,stair,Dstair,ob);
	  //killed(&game,Monsters,mc,M->x,M->y,player);
	  //werase(game);
	  px = M->x;
	  py = M->y;
	  atr = "";
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

      if(store == '+')
	{
	  clrtoeol();
	  refresh();
	  endwin();
	  printf("\n YourWinGame!!!:DDD\n");
	  break;
	}

      if(store == '^')
	{
	  clrtoeol();
	  refresh();
	  endwin();
	  Mfree(&mq);
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
	  
      (M)->s += M->speed;
      Minsert(&mq , M);
    }

  Mfree(&mq);
  
  return false;
       
}

void swapMonster(Monster *Ori , Monster *M , int x , int y , int mc)
{
  int n = isSamePlace(Ori,mc,x,y);
  if( n != -1)
    {
      (Ori+n)->x = M->x;
      (Ori+n)->y = M->y;
    }
}

bool Move( Charactor *M, int mc ,  Monster *Ori ,  point base[21][80],int px, int py, bool check,Player *p , string *atr)
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
	  
	  if(dis[v] < min)
	    {	
	      x = v/80;
	      y = v%80;
	      min = dis[v];
	    }
	  
	}

    }



  // if(M->x == px && M->y == py)
  //   {
  //     return true;
  //   }
  
  if(dis[calculateindex(M->x,M->y)]- dis[calculateindex(x,y)] > 1  )
    {
      dis[calculateindex(x,y)]++;
    }
  else
    {
      
      if(x == px && y == py)
      	{
      	  int d = M->Damage.Roll();
	  //printf("%d\n" , d);
      	  if(p->HP <= d)
      	    return true;
      	  else
      	    {
      	      p->HP = p->HP - d;
	      (*atr) += string(1,((Monster*)M)->Sybol) + " : " +to_string(d) + " , ";
      	      return false;
      	    }
	  
      	}

      
      if(x != 0 && y !=0)
	{
	  swapMonster(Ori,(Monster*)M,x,y,mc);
	  (M->x)= x;
	  (M->y)= y;
	  base[x][y].hardness = 0;
        }
    }
  
  return false;
     
}

bool MoveSix( Charactor *M, int px , int py ,  point base[21][80], int mc , bool check,  Monster *Ori , Player *p , string *atr)
{
  int referx = px - M->x;
  int refery = py - M->y;

  if((referx == 0 && refery ==1) || (referx == 0 && refery ==-1))
    {
      int d = M->Damage.Roll();
      // printf("%d\n" , d);
      if(p->HP <= d)
	return true;
      else
      	{
      	  p->HP = p->HP - d;
	  (*atr) += string(1,((Monster*)M)->Sybol) + " : " + to_string(d) + ", ";
      	  return false;
      	}
    }

  
  if(referx != 0)
    {
      if(referx > 0)
	{
	  
	  if(base[(M->x)+1][M->y].value != ' ' &&  check == false)
	    {
	      swapMonster(Ori,(Monster*)M,(M->x)+1,M->y,mc);
	      M->x = (M->x)+1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if( check == true)
	    {
	      if(base[(M->x)+1][M->y].hardness/85 > 0)
		{
		  base[(M->x)+1][M->y].hardness -= 85;
		}
	      else
		{
		  swapMonster(Ori,(Monster*)M,(M->x)+1,M->y,mc);
		  M->x = (M->x)+1;
		  base[M->x][M->y].hardness = 0;
		}
	    }
	}
      else if(referx < 0)
	{
	  if(base[(M->x)-1][M->y].value != ' ' && check == false)
	    {
	      swapMonster(Ori,(Monster*)M,(M->x)-1,M->y,mc);
	      M->x = (M->x)-1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if(check == true)
	    {
	      if(base[(M->x)-1][M->y].hardness/85 > 0)
		{
		  base[(M->x)-1][M->y].hardness -= 85;
		}
	      else
		{
		  swapMonster(Ori,(Monster*)M,(M->x)-1,M->y,mc);
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
	  if(base[M->x][(M->y)+1].value != ' '  && check == false)
	    {
	      swapMonster(Ori,(Monster*)M,M->x,(M->y)+1,mc);
	      M->y = (M->y) +1;
	      base[M->x][M->y].hardness = 0;
	    }
	  else if( check == true)
	    {
	      if(base[M->x][(M->y)+1].hardness/85 > 0)
		{
		  base[M->x][(M->y)+1].hardness -=85;
		}
	      else
		{
		  swapMonster(Ori,(Monster*)M,M->x,(M->y)+1,mc);
		  M->y = (M->y)+1;
		  base[M->x][M->y].hardness = 0;
		}
	    }
	}
      else if(refery <0)
	{
	  if(base[M->x][(M->y)-1].value != ' ' && check == false)
	    {
	      swapMonster(Ori,(Monster*)M,M->x,(M->y)-1,mc);
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
		  swapMonster(Ori,(Monster*)M,M->x,(M->y)-1,mc);
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

bool Moverand( Charactor *M , point base[21][80] , int mc , bool check , int px ,int py, Monster *Ori,Player *p , string *atr)
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

  if(x ==px && y == py)
    {
      int d = M->Damage.Roll();
      // printf("%d\n" , d);
      if(p->HP <= d)
	return true;
      else
      	{
      	p->HP = p->HP - d;
	(*atr) += string(1,((Monster*)M)->Sybol) + " : " + to_string(d) + ", ";
      	return false;
      	}
    }

  
  if(check == false)
    {

      if(base[x][y].hardness == 0)
	{
	 swapMonster(Ori,(Monster*)M,x,y,mc);
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
	      swapMonster(Ori,(Monster*)M,x,y,mc);
	      M->x = x;
	      M->y = y;
	      base[M->x][M->y].hardness = 0;
	    }
	}
    }
   
  // if(M->x == px && M->y == py)
  //   {
  //     return true;
  //   }

  return false;
}

bool IsAround(int x, int y, int px , int py)
{
  if(x>px +3 || x < px -3)
    return false;
  
  if(y>py+3 ||  y < py-3)
    return false;


  return true;

}
