#include "map.h"
#include "RLfile.h"

void WriteFile(int size, int playerloco[2] ,  point Base[21][80],  room *r , char *fname)
{


  if(size != 0)
    {
      int i,j;
      uint8_t roomp[size*4];
      uint8_t *rloco = roomp;
      int n = 0;
      for(i = 0 ; i< (size*4) ; i +=4)
	{
	  *(rloco+i) = (r+n)->y; 
	  *(rloco+(i+1)) = (r+n)->x;
	  *(rloco+(i+2))= (r+n)->rw;
	  *(rloco+(i+3))=(r+n)->rl;
	  n++;
	}



      FILE *f;
      char str[12] = {'R','L','G','3','2','7','-','F','2','0','1','8'};
      uint32_t *Filev;
      uint32_t *fz;


		
      f = fopen(fname , "wb");

		

      uint8_t ploco[] = {(uint8_t)playerloco[1] , (uint8_t)playerloco[0]};
      uint8_t hardness[1680];
      uint8_t *hs = hardness;



      for(i = 0 ; i < 21;i++)
	{
	  for(j = 0 ;j < 80 ; j ++)
	    {
	      *(hs+(80*i)+j) = Base[i][j].hardness;
	    }
	}

      Filev = (uint32_t*)malloc(sizeof(uint32_t));
      fz = (uint32_t*)malloc(sizeof(uint32_t));

      *Filev = 0;
      *fz = (1702+(size*4));

      ConvertToEndian(Filev);
      ConvertToEndian(fz);

      fwrite(str, 1 , 12 , f);
      fwrite(Filev , 4 , 1 , f);
      fwrite(fz , 4, 1 ,f);
      fwrite(ploco, 1 , 2 , f);
      fwrite(hardness,1,1680,f);
      fwrite(roomp,1, size*4 ,f);

      fclose(f);

    }
}

void loadFile( point p[21][80],  room *r , int *size , char *fname ,int PlayLocation[2])
{

  FILE *read;
  read = fopen("hello.rlg327" , "rb");

  int i,j ,n;


  char str[12];
  uint32_t Filec;
  uint32_t rz;
  uint8_t hardness[1680];
  uint8_t ploco[2];
  //uint8_t hs = hardness;
  //uint8_t po = ploco;   

  fread(str , 1 ,12,read);
  fread(&Filec , 4 , 1 , read);
  fread(&rz , 4, 1 ,read);
  fread(ploco,1,2,read);
  fread(hardness,1,1680,read);

  hardnessMerge(p,hardness);

  ConvertToHost(&rz);
  ConvertToHost(&Filec);


  *size = ((int)(rz)-1702)/4;
  uint8_t roomp[*size*4];
  uint8_t *rp = roomp;

  fread(rp,1,(*size)*4,read);

  r = ( room*)malloc(*size*sizeof( room));

  n = 0;

  for(i = 0 ; i < (*size)*4 ; i +=4)
    {
      (r+n)->x = (int)roomp[i];
      (r+n)->y = (int)roomp[i+1];
      (r+n)->rl = (int)roomp[i+2];
      (r+n)->rw = (int)roomp[i+3];
      n++;
    }


  // point temp[21][80];
  //initialBase (temp);

  n= 0;

  while(n != *size)
    {
      for(i = 0 ; i < (r+n)->rw ; i ++)
	{
	  for(j = 0 ; j<(r+n)->rl; j ++)
	    {
	      p[((r+n)->y)+i][((r+n)->x)+j].value = '.';
	      p[((r+n)->y)+i][((r+n)->x)+j].x = i;
	      p[((r+n)->y)+i][((r+n)->x)+j].y = j;

	    }
	}
      n++;
    }



  p[ploco[1]][ploco[0]].value = '@';
  p[ploco[1]][ploco[0]].x = ploco[1];
  p[ploco[1]][ploco[0]].y = ploco[0];

  PlayLocation[0]=ploco[1];
  PlayLocation[1]=ploco[0];

  for (i = 0; i < 21; i++)
    {
      for (j = 0; j < 80; j++)
	{
	  printf ("%c", p[i][j].value);
	}
      printf ("\n");
    }


  fclose(read);

} 


void ConvertToEndian(uint32_t *i)
{
  *i = htobe32(*i);
}

void ConvertToHost(uint32_t *i)
{
  *i = be32toh(*i);
}

void hardnessMerge( point p[21][80] , uint8_t hardness[1680])
{

  int i,j;
  for(i = 0 ; i < 21 ; i ++)
    {
      for(j = 0 ; j < 80 ; j ++)
	{
	  p[i][j].hardness  = (int) hardness[(80*i)+j];
	  if(p[i][j].hardness == 255)
	    {
	      if(i==0 || i == 20)
		{
		  p[i][j].value = '-';
		  p[i][j].x =i;
		  p[i][j].y = j;
		}
	      else if(j==0 || j==79)
		{
		  p[i][j].value = '|';
		  p[i][j].x =i;
		  p[i][j].y = j;
		}
	    }
	  else if(p[i][j].hardness == 0)
	    {
	      p[i][j].value = '#';
	      p[i][j].x =i;
	      p[i][j].y = j;
	    }
	  else
	    {
	      p[i][j].value = ' ';
	      p[i][j].x =i;
	      p[i][j].y = j;
	    }

	}
    }


}
