#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include<sys/file.h>

#include <stdio.h>

using namespace std;

void error_and_die(const char *msg){     
     perror(msg);
     exit(EXIT_FAILURE);
}
int direction(int x1 ,int y1)          //decide the boat direction
{
   int dir;   //direction
   if(x1 ==0)
   {  
      if(y1 == 0)
      	 dir = rand()%2+1;
      else if(y1 == 3)
         dir = rand()%2;
      else
      	 dir = rand()%3;
   }
   else if(x1 == 3)
   {
      if(y1 == 0)
      	 dir = rand()%2+2;
      else if(y1 == 3)
      {
      	 int a;
      	 a = rand()%2;
      	 if(a == 0)
      	    dir = 0;
      	 else
      	    dir = 3;
      }
      else
      {
      	int a;
      	a = rand()%3;
      	if(a == 0)
      	  dir = 0;
      	else if (a == 1)
      	  dir = 2;
      	else
      	  dir = 3;
      }
   }
   else if(y1 == 0 && x1 != 0 && x1 != 3 )
       dir = rand()%3+1;
   else if (y1 == 3 && x1 != 0 && x1 != 3 )
   {
   	int a;
   	a = rand()%3;
   	if(a == 0)
   	  dir = 0;
   	else if (a == 1)
   	  dir = 1;
   	else
   	  dir = 3;  
   }
   else
      dir = rand()%4;
   return dir;
}

void ship(int dir , int x1, int y1 ,int &x2 , int &y2) 
{                                        //decide boat second point
   x2 = x1;
   y2 = y1;
   switch (dir)
   {
      case 0:
      	y2--;
      	break;
      case 1:
        x2++;
        break;
      case 2:
        y2++;
        break;
      case 3:
        x2--;
        break;
   }
}

bool beatk(int x1,int y1,int x2,int y2,int hitx,int hity,int &atkp)
{                                            //judging whether to be hit
   if(hitx == x1 && hity == y1)
   {
        atkp = 1;
    	return 1;
   }
   else if(hitx == x2 && hity == y2)
   {
         atkp = 2;
      	 return 1;
   }
   else
   {
      return 0;
   }
return 0;
}

void report(int &hitnum,int hit,int pi,string str,int &down,int atkp,int &atkp1)
{					//report the state after shooting
   if(hit == 1)
   {
      hitnum++;
      if(hitnum == 2)
      {
         if(atkp == atkp1)
         {
            hitnum--;
            cout<<"["<<pi<<" "<<str<<"]"<<": hit"<<endl;
         }
         else
         {
      	    cout<<"["<<pi<<" "<<str<<"]"<<": hit and sinking"<<endl;
      	    down++;
      	 }
      }
      else if(hitnum == 1)
      {
         atkp1 = atkp;
         cout<<"["<<pi<<" "<<str<<"]"<<": hit"<<endl;
      }
   }
   else if(hit == 0)
   {
      cout<<"["<<pi<<" "<<str<<"]"<<": missed"<<endl;
   }
}

void atk (int &shotx, int &shoty, int &shotnum, int pi , string str)
{
  shotx = rand()%4;
  shoty = rand()%4;
  shotnum++;
  cout<<"["<<pi<<" "<<str <<"]: bombing("<<shotx<<","<<shoty<<")"<<endl;
}

int main(int argc, char *argv[])
{
  int r;
  signal(SIGCHLD,SIG_IGN); 
 
  const char *memname="sample";
  const size_t region_size = sysconf(_SC_PAGE_SIZE);
 
  int fd = shm_open(memname, O_CREAT|O_TRUNC|O_RDWR,0666);
  if(fd==-1) 
     error_and_die("shm_open");
 
  r=ftruncate(fd,region_size);
  
  if(r!=0)  
     error_and_die("ftruncate");
 
  int *shm=(int *)mmap(0,region_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  
  if(shm == MAP_FAILED) 
      error_and_die("mmap");
      
//------------------------------share memory-------------------------        
    
    int pid ;
    pid = fork();
    char ps = *argv[1],cs = *argv[2];
    int parseed = (int)ps -'0',chiseed = (int)cs -'0';
    shm[2] = 0;  // judging whether to jump
    shm[3] = 0;  // save pi
    shm[4] = 0;  // save shotnum
    shm[5] = 0;  // step of call Random seed and The gunboat
    shm[6] = 0;  // step of shot
    shm[7] = 0;  // judging whether to end
    
  if(pid == 0)  //child
  {
     string str = ("Child");
     int x1, y1, x2 = 0, y2 = 0,dir,pi = getpid();
     srand(time(NULL) ^ chiseed);
     while(true)  //the while loop is telling information of child
     {
        if(shm[5] == -1)
        {
           cout<<"["<<pi<<" Child" <<"]"<<": Random Seed "<<argv[2]<<endl;
           shm[5] = -2;
           //Random seed
        }
        else if(shm[5] == -3)
        {
           x1 = rand()%4;
           y1 = rand()%4;
           dir = direction(x1,y1);
           ship(dir, x1, y1, x2, y2);
           cout<<"["<<pi<<" Child" <<"]"<<": The gunboat: ";
           cout<<"("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<")"<<endl;
           shm[5] = -4;
        }
        if(shm[5] == -4)
          break;
       
     //tell where boat
     }
     
     
     int hitnum = 0, down=0, shotnum =0 ,atkp =0,atkp1=0;
     while(true)  		// atk and be atked loop
     {
        if(shm[6] == 1)
        {
           int hitx = shm[0],hity = shm[1];
           bool hit =0;
           hit = beatk(x1, y1, x2, y2, hitx, hity,atkp);
           report(hitnum, hit, pi, str,down,atkp,atkp1);
           
           //be atked
           if(down == 1)
           {
              shm[2] = 1;
              break;
           }
           //judging whether to be atked
           
           int shotx=0,shoty=0;
           atk(shotx, shoty, shotnum, pi, str);
           shm[0] = shotx ;
           shm[1] = shoty ;
           shm[6] = -1;
           //atk
        }
        if(shm[2] == 1)
        {
           shm[3] = pi;
           shm[4] = shotnum;
           shm[7] = 1;
           break;
        }
        // judging whether to end 
        
     } 
     exit(0);
  }
  
  else //parent
  {
     string str = ("Parent");
     int x1, y1, x2 = 0, y2 = 0,dir , pi = getpid();
     srand(time(NULL) ^parseed);
     cout<<"["<<pi<<" Parent" <<"]"<<": Random Seed "<<argv[1]<<endl;
     //Random seed
     
     while(true)  //the while loop is telling information of parent
     {
        if(shm[5] == 0)
        {
           shm[5]--;
        }
        else if(shm[5] == -2)
        {
           x1 = rand()%4;
           y1 = rand()%4;
           dir = direction(x1,y1);
           ship(dir, x1, y1, x2, y2);
           cout<<"["<<pi<<" Parent" <<"]"<<": The gunboat: ";
           cout<<"("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<")"<<endl;
           shm[5] = -3;
        }
        //tell where boat
        
        else if(shm[5] == -4)
           break;
     }
     
     
     
     int hitnum = 0, down=0, shotnum =0 ,atkp = 0,atkp1=0;
     while(true) 		// atk and be atked loop
     {
        if(shm[6] == 0)
        {
           int shotx=0,shoty=0;
           atk(shotx, shoty, shotnum, pi, str);
           shm[0] = shotx ;
           shm[1] = shoty ;
           shm[6] = 1;
           //atk
           
        }
        if(shm[2] == 1)
        {
           shm[3] = pi;
           shm[4] = shotnum;
           shm[7] = 1;
           break;
        }
        // judging whether to end 
     
        else if(shm[6] == -1)
        {
           int hitx = shm[0],hity = shm[1];
           bool hit =0;
           hit = beatk(x1, y1, x2, y2, hitx, hity,atkp);
           report(hitnum, hit, pi, str,down,atkp,atkp1);
           shm[6] = 0;
           //be atked
          
           if(down == 1)
           {
              shm[2] = 1;
              break;
           }
           //judging whether to be atked
           
        }
     }
     while(true)
     {
        if(shm[2] == 1 && shm[7] == 1)
        {
           cout<<"["<<pi<<" Parent" <<"]: "<<shm[3]<<" wins with "<<shm[4];
           cout<<" bombs"<<endl;
           break;
        }
        
        // get grade
     }   
        
  } 
     
     close(fd);
     r = munmap(shm,region_size);
     if(r!=0) 
     	error_and_die("shm_unlink");
     return 0;
     //end share memory
     
}
