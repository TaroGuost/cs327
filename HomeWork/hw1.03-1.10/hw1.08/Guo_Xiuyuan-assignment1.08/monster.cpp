#include "monster.h"

void objects :: setSybol()
{
  if(type.compare("WEAPON") == 0)
    Sybol = '|';
  else if(type.compare("OFFHAND") == 0)
    Sybol = ')';
  else if(type.compare("RANGED") == 0)
    Sybol = '}';
  else if(type.compare("ARMOR") == 0)
    Sybol = '[';
  else if(type.compare("HELMET") == 0)
    Sybol = ']';
  else if(type.compare("CLOAK") == 0)
    Sybol = '(';
  else if(type.compare("GLOVES") == 0)
    Sybol = '{';
  else if(type.compare("BOOTS") == 0)
    Sybol = '\\';
  else if(type.compare("RING") == 0)
    Sybol = '=';
  else if(type.compare("AMULET") == 0)
    Sybol = '"';
  else if(type.compare("LIGHT") == 0)
    Sybol = '_';
  else if(type.compare("SCROLL") == 0)
    Sybol = '~';
  else if(type.compare("BOOK") == 0)
    Sybol = '?';
  else if(type.compare("FLASK") == 0)
    Sybol = '!';
  else if(type.compare("GOLD") == 0)
    Sybol = '$';
  else if(type.compare("AMMUNITION") == 0)
    Sybol = '/';
  else if(type.compare("FOOD") == 0)
    Sybol = ',';
  else if(type.compare("WAND") == 0)
    Sybol = '-';
  else if(type.compare("CONTAINER") == 0)
    Sybol = '%';
  else if(type.compare("STACK") == 0)
    Sybol = '&';
  else
    Sybol = '*';
  
}

void objects :: assignBase(point base[21][80] , int n)
{
  base[x][y].ob = Sybol;
  base[x][y].position = n;
}

int objects :: SelectColor()
{
  for(int i = 0 ; i <8 ; i++)
    {
      if(c[i] != BLACK)
	return c[i];
    }

  return WHITE;
}

void dices :: setup(vector<string> s )
{
  stringstream geek(s.at(0));
  geek >>base;
  vector<string> temp = Split(s.at(1) , 'd');
  stringstream gee(temp.at(1));
  gee >>side;
  stringstream ge(temp.at(0));
  ge >>Amount;
}

int dices :: Roll()
{
  if(Amount ==0 || side ==0)
    return 0;
  
  int temp[Amount];
  int total = 0;
  
  for(int i = 0 ; i < Amount ; i++)
    {
      temp[i] = (rand()%side)+1;
      total += temp[i];
    }

  return base + total;
}

string dices :: toString()
{
  string temp;

  temp = to_string(base) + "+" + to_string(Amount) + "d" + to_string(side);

  return temp;
}

void dice :: adddice(dices temp)
{
  side += temp.side;
  Amount += temp.Amount;
  base += temp.base;
}

void dice :: takeoff(dice temp)
{
  side -= temp.side;
  Amount += temp.Amount;
  base += temp.base;
}

char Mtype(int a, int b , int c ,int d)
{
  long int bin , hex= 0 , i =1 , temp;
  bin = (rand()+1)%a + ((rand()+1)%b*10)+ ((rand()+1)%c*100)+ ((rand()+1)%d*1000);
  while(bin != 0)
    {
      temp = bin%10;
      hex += temp*i;
      i = i*2;
      bin = bin/10;
    }

    
  return hex;
    
    
}

int Monster :: usefulColor()
{
  for(int i = 0 ; i < 8 ; i++)
    {
      if(c[i] != 0)
	{
	  return c[i];
	}
    }

  return GREEN;
}

void Monster ::setType()
{
  long int bin =0, hex=0, i = 1 , temp=0;
  
  for(int i = 0 ; i < 9 ; i ++)
    {
      if(a[i]<=0)
	continue;
      else
	bin+= a[i];
     }
  
  while(bin != 0)
    {
      temp = bin%10;
      hex += temp*i;
      i = i*2;
      bin = bin/10;
    }

  type = hex;
}

string Monster :: displayAbil()
{
  string temp = "";

  for(int i = 0 ; i < 9 ; i++)
    {
      if(a[i] == NONE)
	continue;
      else if(a[i] == SMART)
	temp += "SMART ";
      else if(a[i] == TUNNEL)
	temp += "TUNNEL ";
      else if(a[i] == ERRATIC)
	temp += "ERRATIC ";
      else if(a[i] == TELE)
	temp += "TELE ";
      else if(a[i] == PASS)
	temp += "PASS ";
      else if(a[i] == PICKUP)
	temp += "PICKUP ";
      else if(a[i] == DESTORY)
	temp += "DESTORY ";
      else if(a[i] == UNIQ)
	temp += "UNIQ ";
      else if(a[i] == BOSS)
	temp += "BOSS ";
    }

  return temp;

}

void tempc(Monster *M , int size)
{
  

  for(int i = 0 ; i < size ; i ++)
    {
      cout << "Name " << (M+i)->name << endl;
      cout << "DESC :" << (M+i)->Desc;
      cout << "COLOR " << (M+i)->c[0] <<endl;
      cout << "Speed " << (M+i)->speed << endl;
      cout << "Ability: " << (*(M+i)).displayAbil() << endl;
      cout << "HP " << (M+i)->HP <<endl;
      cout << "Damage " << (M+i)->Damage.toString() << endl;
      cout << "SYMB " << (M+i)->Sybol << endl;
      cout << "RRTY: " << (M+i)->RRTY <<endl;
      cout << "Type: " ;
      printf("%lx\n\n" , (M+i)->type);
    }

  cout << "size of array: " << sizeof(*M)/sizeof(Monster);
  
}

void GMonster( room *rooms ,  Monster *Monsters , int size ,  point base[21][80] , int mc , Monster *Cala , int Msize)
{
  int MonsterCount = 0;
  
  while(MonsterCount < mc)
    {
      int random = (rand()%size);
      int randd = (rand()%Msize);
      if(!IfPC(rooms+random, base) && checkCala(Cala+randd))
	{
	  *(Monsters+MonsterCount) = *(Cala+randd); 
	  (Monsters+MonsterCount)->x = ((rooms+random)->x)+(rand()%(rooms+random)->rl);
	  (Monsters+MonsterCount)->y = ((rooms+random)->y)+(rand()%(rooms+random)->rw);
	  MonsterCount++;
	}
    }
}

bool checkCala(Monster *M)
{
  if(M->RRTY >= rand()%100)
    return true;
  else
    return false;
}

vector<string> Split(string s , char c)
{
  vector<string> result;
  string temp;
  istringstream tokenStream(s);
  while(getline(tokenStream,temp,c))
    {
      result.push_back(temp);
    }

  return result;
}

void setCount(string s , int *count)
{
  if(s.compare("BEGIN")==0)
    *count = 0;
  else if(s.compare("NAME") == 0)
    *count = 1;
  else if(s.compare("SYMB") == 0)
    *count = 2;
  else if(s.compare("COLOR") == 0)
    *count = 3;
  else if(s.compare("DESC") == 0)
    *count = 4;
  else if(s.compare("SPEED") == 0)
    *count = 5;
  else if(s.compare("DAM") == 0)
    *count = 6;
  else if(s.compare("HP") == 0)
    *count = 7;
  else if(s.compare("ABIL") == 0)
    *count = 8;
  else if(s.compare("RRTY") == 0)
    *count = 9;
  else if(s.compare("END") == 0)
    *count = -1;
  else if(s.compare("TYPE") == 0)
    *count = 10;
  else if(s.compare("WEIGHT") == 0)
    *count = 11;
  else if(s.compare("ATTR") == 0)
    *count = 12;
  else if(s.compare("DODGE")==0)
    *count = 13;
  else if(s.compare("VAL")==0)
    *count = 14;
  else if(s.compare("DEF")==0)
    *count = 15;
  else if(s.compare("ART")==0)
    *count = 17;
  else if(s.compare("HIT") ==0)
    *count = 16;
}

int ImportMonster(Monster **M , string Filename)
{
  ifstream File;
  File.open(Filename);

  int count = -1;
  int size =0;
  string x;
  vector<string> content;
  vector<string> s;
  dices di;

  
  while(getline(File,x))
    {
      
      if(count != 4 && x != "")
	{
	  content = Split(x , ' ');
	  setCount(content.at(0),&count);
	}
      else
	{
	  if(x.compare(".")== 0)
	    {
	      count = -1;
	      continue;
	    }
	
	}
      
      if(count >4 && count < 8)
	{
	  int side , amount;
	  s = Split(content.at(1) , '+');
	  di.setup(s);
	  di.Roll();
	}
      
      switch(count)
	{
	case -1:
	  break;
	case 0:
	  size++;
	  if(size != 1)
	    *M = (Monster*)realloc(*M,size*sizeof(Monster));
	  break;
	case 1:
	  for(int i = 1 ; i < content.size() ; i++)
	    (*M+size-1)->name +=  content.at(i) + " ";
	  break;
	case 2:
	  (*M+size-1)->Sybol = content.at(1)[0];
	  break;
	case 3:
	   for(int i = 1 ; i < content.size() ; i++)
	    {
	      if(content.at(i).compare("BLACK") == 0)
		(*M+size-1)->c[i-1] = BLACK;
	      else if(content.at(i).compare("RED") == 0 )
		(*M+size-1)->c[i-1] = RED;
	      else if(content.at(i).compare("GREEN") == 0)
		(*M+size-1)->c[i-1] = GREEN;
	      else if(content.at(i).compare("BLUE") == 0)
		(*M+size-1)->c[i-1] = BLUE;
	      else if(content.at(i).compare("MAGENTA") == 0)
		(*M+size-1)->c[i-1] = MAGENTA;
	      else if(content.at(i).compare("CYAN") == 0)
		(*M+size-1)->c[i-1] = CYAN;
	      else if(content.at(i).compare("WHITE") == 0)
		(*M+size-1)->c[i-1] = WHITE;
	      else if(content.at(i).compare("YELLOW") == 0)
		(*M+size-1)->c[i-1] = YELLOW;
	    }
	  break;
	case 4:
	  if(x.compare(".") != 0
	     && x.compare("DESC") != 0)
	    {
	      (*M+size-1)->Desc += x;
	      (*M+size-1)->Desc.push_back('\n');
	    }
	  break;
	case 5:
	  (*M+size-1)->speed = di.Roll();
	  break;
	case 6:
	  (*M+size-1)->Damage =di;
	  break;
	case 7:
	  (*M+size-1)->HP = di.Roll();
	  break;
	case 8:
	  for(int i = 1 ; i < content.size() ; i++)
	    {
	      if(content.at(i).compare("SMART") == 0)
		(*M+size-1)->a[i-1] = SMART;
	      else if(content.at(i).compare("TELE") ==0 )
		(*M+size-1)->a[i-1] = TELE;
	      else if(content.at(i).compare("TUNNEL") == 0)
		(*M+size-1)->a[i-1] = TUNNEL;
	      else if(content.at(i).compare("ERRATIC") == 0)
		(*M+size-1)->a[i-1] = ERRATIC;
	      else if(content.at(i).compare("PICKUP") == 0)
		(*M+size-1)->a[i-1] = PICKUP;
	      else if(content.at(i).compare("PASS") == 0)
		(*M+size-1)->a[i-1] = PASS;
	      else if(content.at(i).compare("DESTORY") == 0)
		(*M+size-1)->a[i-1] = DESTORY;
	      else if(content.at(i).compare("UNIQ") == 0)
		(*M+size-1)->a[i-1] = UNIQ;
	      else if(content.at(i).compare("BOSS") == 0)
		(*M+size-1)->a[i-1] = BOSS;
	    }
	  
	  (*(*M+size-1)).setType();
	  break;
	case 9:
	  stringstream g(content.at(1));
	  g >> (*M+size-1)->RRTY;
	  break;
	  
	}
      
    }

  
  File.close(); 
  
  return size;

  
}

bool IfPC( room *r ,  point base[21][80])
{
  int i,j;

  for(i = (r->x) ; i < ((r->x)+(r->rw)) ; i ++)
    {
      for(j = (r->y) ; j < ((r->y)+(r->rl)); j ++)
	{
	  if(base[i][j].value == '@')
	    {
	      return true;
	    }
	}
    }
  
  return false;
}


void addPlayer( Player *p  , int px , int py)
{
  
  p->x = px;
  p->y = py;
  p->type = '@';
  p->speed = 10;
  p->s = 0;
  p->Dead = false;
}

void ImportObject(vector<objects> *O , string name)
{
  ifstream F;
  F.open(name);

  int count = -1;
  string x;
  vector<string> content;
  vector<string> s;
  dices di;
  objects *temp = new objects;
  
  while(getline(F,x))
    {

      if(x == "")
	continue;
      
      if(count != 4 && x != "")
	{
	  content = Split(x , ' ');
	  setCount(content.at(0),&count);
	}
      else
	{
	  if(x.compare(".")== 0)
	    {
	      count = -2;
	      continue;
	    }
	
	}
      
      if((count >4 && count < 8) ||(count > 10 && count < 16))
	{
	  int side , amount;
	  s = Split(content.at(1) , '+');
	  di.setup(s);
	  di.Roll();
	}
      
      switch(count)
	{
	case -1:
	  (*O).push_back(*temp);
	  //delete temp;
	  //temp = new objects;
	  break;
	case 0:
	  temp = new objects;
	  break;
	case 1:
	  for(int i = 1 ; i < content.size() ; i++)
	    (temp)->name +=  content.at(i) + " ";
	  break;
	case 3:
	   for(int i = 1 ; i < content.size() ; i++)
	    {
	      if(content.at(i).compare("BLACK") == 0)
		(temp)->c[i-1] = BLACK;
	      else if(content.at(i).compare("RED") == 0 )
		(temp)->c[i-1] = RED;
	      else if(content.at(i).compare("GREEN") == 0)
	        (temp)->c[i-1] = GREEN;
	      else if(content.at(i).compare("BLUE") == 0)
	        (temp)->c[i-1] = BLUE;
	      else if(content.at(i).compare("MAGENTA") == 0)
		(temp)->c[i-1] = MAGENTA;
	      else if(content.at(i).compare("CYAN") == 0)
		(temp)->c[i-1] = CYAN;
	      else if(content.at(i).compare("WHITE") == 0)
		(temp)->c[i-1] = WHITE;
	      else if(content.at(i).compare("YELLOW") == 0)
		(temp)->c[i-1] = YELLOW;
	    }
	  break;
	case 4:
	  if(x.compare(".") != 0
	     && x.compare("DESC") != 0)
	    {
	      (temp)->Desc += x;
	      (temp)->Desc.push_back('\n');
	    }
	  break;
	case 5:
	  (temp)->speed = di.Roll();
	  break;
	case 6:
	  (temp)->damage =di;
	  break;
	case 7:
	  (temp)->HP = di.Roll();
	  break;
	case 9:
	  {
	    stringstream gg(content.at(1));
	    gg >> (temp)->RRTY;
	  }
	  break;
	case 10: 
	  (temp)->type = content.at(1);
	  (temp)->setSybol();
	  cout << "TYPE: " << content.at(1) <<endl;
	  break;
	case 11: 
	  (temp)->weight = di.Roll();
	  break;
	case 12:
	  (temp)->attr = di.Roll();
	  break;
	case 13: 
	  (temp)->dodge = di.Roll();
	  break;
	case 14: 
	  (temp)->val = di.Roll();
	  break;
	case 15: 
	  (temp)->def = di.Roll();
	  break;
	case 17:
	  (temp)->hit = di.Roll();
	  break;
	case 16: 
	  if(content.at(1).compare("TRUE") ==0)
	    (temp)->art = true;
	  else
	    (temp)->art = false;
	  break;
	}
      
    }

  F.close();
 
}

void printobject(vector<objects> *O)
{
  
  for(int i = 0 ; i < (*O).size() ; i++)
    {
      cout<< "Object name: "<<(*O).at(i).name << endl;
      cout << "Object Desc: "<< (*O).at(i).Desc <<endl <<endl;
      cout << "Object sybol: "<< (*O).at(i).Sybol << endl;
      cout << "isArt: " << (*O).at(i).art << endl;
      cout << "Type:" << (*O).at(i).type << endl <<endl;
    }
}

void Gobject(vector<objects> *ob, room *rooms , int size  , point base[21][80] , vector<objects> O)
{
  int n =0;
  int count = 10 + rand()%5;
  bool check = false;
  while(n < 11)
    {
      int random = (rand()%size);
      int po = rand()%O.size();
      int x = ((rooms+random)->x)+(rand()%(rooms+random)->rl);
      int y = ((rooms+random)->y)+(rand()%(rooms+random)->rw);
      if(base[x][y].ob == '.' && (rand()%100 <= O.at(po).RRTY) && base[x][y].position ==-1)
	{
	  if(O.at(po).art && check || (O.at(po).Sybol == '*'))
	     {
	       continue;
	     }
	   else if(O.at(po).art)
	     {
	       check = true;
	     }
	   
	  (*ob).push_back(O.at(po));
	  ((*ob).at(n)).x = x;
	  ((*ob).at(n)).y = y;
	  ((*ob).at(n)).assignBase(base , n);
	  n++;
	}
    }
}
