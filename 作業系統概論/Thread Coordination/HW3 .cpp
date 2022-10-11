#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

bool dis[3] = {false};
bool p1_item[3] = {false,false,true};
bool p2_item[3] = {false};
bool p3_item[3] = {false};
bool check[3] = {true};
int sum =0;
int dis_item[3] = {0};
pthread_mutex_t lock;
int seed;

struct pi_item
{
   string pi;
   int num = 0;
};
pi_item pdc[3];
bool compare(pi_item p1,pi_item p2)
{
   return p1.num > p2.num;
}

int trans(string value)
{
    unsigned int deg = value.size() - 1,sum=0;
    for(unsigned int i=0;i<value.size();i++)
    {
        int tmp=(int)value[i]-'0';
        sum = sum+tmp * pow(10,deg);
        deg--;
    }
    return sum;
}
void* dispatcher(void* arg)
{
   srand (time(NULL)^seed);
   while(sum != 50)
   {
      pthread_mutex_lock(&lock);
      if(sum == 50 || check[0]==false || check[1]==false || check[2]==false)
      {
      	  pthread_mutex_unlock(&lock);
      	  continue;
      }
      int item = 0;
      while(true)
      {
         if(dis[0]==true && dis[1]==true && dis[2]==true)
            break;
         item = rand()%3;
         if(dis[item] == false)
         {
            dis[item] = true;
            break;
         }
         else
      	    continue;
      }
      
      
      switch (item)
      {
         case 0:
            dis_item[0]++;
      	    cout<<"Dispatcher: propeller"<<endl;
      	    break;
         case 1:
            dis_item[1]++;
      	    cout<<"Dispatcher: battery"<<endl;
      	    break;
         case 2:
            dis_item[2]++;
      	    cout<<"Dispatcher: aircraft"<<endl;
            break;
         default:
            break;
      }
      check[0]=false;
      check[1]=false;
      check[2]=false;
      pthread_mutex_unlock(&lock);
   }
      return NULL;
}

void* producer(void* arg)
{
   int p_num = *(int *)arg;
   int p1_sum = 0, p2_sum = 0,p3_sum = 0;
   pdc[0].pi = "producer 1 (aircraft): ";
   pdc[1].pi = "producer 2: ";
   pdc[2].pi = "producer 3: ";
   while(sum != 50)
   {
      if(p_num == 1)
      {
         bool take=false;
      	 int item_num;
      	 pthread_mutex_lock(&lock);
      	 check[0] = true;
      	 if(sum == 50)
      	 {
      	    pthread_mutex_unlock(&lock);
      	    break;
      	 }
      	 for(int i=0;i<2;i++)
      	 {
      	    if(dis[i] == true && p1_item[i] == false)
      	    {
      	    	dis[i]=false;
      	    	p1_item[i] = true;
      	    	item_num = i;
      	    	take = true;
      	    	break;
      	    }
      	    else
      	        continue;
      	 }
      	 
      	 if(take == true)
      	 {
      	    switch (item_num)
      	    {
      	       case 0:
      	       	  cout<<"Producer 1 (aircraft): get propeller"<<endl;
      	       	  break;
      	       case 1:
      	       	  cout<<"Producer 1 (aircraft): get battery"<<endl;
      	       	  break;
      	    }
      	 }
      	 if(p1_item[0]==true && p1_item[1]==true && p1_item[2]==true)
      	 {
      	     p1_sum++;
      	     pdc[0].num++;
      	     sum++;
      	     cout<<"Producer 1 (aircraft): OK "<<p1_sum<<" drone(s)"<<endl;
      	     p1_item[0] = false;
      	     p1_item[1] = false;
      	     p1_item[2] = true;
      	 }
      	 pthread_mutex_unlock(&lock);
      }
      else if(p_num == 2)
      {
      	 bool take=false;
      	 int item_num;
      	 pthread_mutex_lock(&lock);
      	 check[1] = true;
      	 if(sum == 50)
      	 {
      	    pthread_mutex_unlock(&lock);
      	    break;
      	 }
      	 
      	 for(int i=0;i<3;i++)
      	 {
      	    if(dis[i] == true && p2_item[i] == false)
      	    {
      	    	dis[i]=false;
      	    	p2_item[i] = true;
      	    	item_num = i;
      	    	take = true;
      	    	break;
      	    }
      	    else
      	        continue;
      	 }
      	 
      	 if(take == true)
      	 {
      	    switch (item_num)
      	    {
      	       case 0:
      	       	  cout<<"Producer 2: get propeller"<<endl;
      	       	  break;
      	       case 1:
      	       	  cout<<"Producer 2: get battery"<<endl;
      	       	  break;
      	       case 2:
      	       	  cout<<"Producer 2: get aircraft"<<endl;
      	       	  break;
      	    }
      	 }
      	 if(p2_item[0]==true && p2_item[1]==true && p2_item[2]==true)
      	 {
      	     p2_sum++;
      	     sum++;
      	     pdc[1].num++;
      	     cout<<"Producer 2: OK "<<p2_sum<<" drone(s)"<<endl;
     	     p2_item[0] = false;
      	     p2_item[1] = false;
      	     p2_item[2] = false;
      	 }
      	 pthread_mutex_unlock(&lock);
      }
      	 
      else
      {
      	 bool take=false;
      	 int item_num;
      	 pthread_mutex_lock(&lock);
      	 check[2] = true;;
      	 if(sum == 50)
      	 {
      	    pthread_mutex_unlock(&lock);
      	    break;
      	 }
      	 
      	 for(int i=0;i<3;i++)
      	 {
      	    if(dis[i] == true && p3_item[i] == false)
      	    {
      	    	dis[i]=false;
      	    	p3_item[i] = true;
      	    	item_num = i;
      	    	take = true;
      	    	break;
      	    }
      	    else
      	        continue;
      	 }
      	 
      	 if(take == true)
      	 {
      	    switch (item_num)
      	    {
      	       case 0:
      	       	  cout<<"Producer 3: get propeller"<<endl;
      	       	  break;
      	       case 1:
      	       	  cout<<"Producer 3: get battery"<<endl;
      	       	  break;
      	       case 2:
      	       	  cout<<"Producer 3: get aircraft"<<endl;
      	       	  break;
      	    }
      	 }
      	 if(p3_item[0]==true && p3_item[1]==true && p3_item[2]==true)
      	 {
      	     p3_sum++;
      	     sum++;
      	     pdc[2].num++;
      	     cout<<"Producer 3: OK "<<p3_sum<<" drone(s)"<<endl;
      	     p3_item[0] = false;
      	     p3_item[1] = false;
      	     p3_item[2] = false;
      	 }
      	 pthread_mutex_unlock(&lock);
      }
   }
   return NULL;
}

int main(int argc ,char* argv[])
{
   string ran;
   char m_t = (*argv[1]);
   int mode = (int)m_t-'0';
   if(mode == 1)
      cout<<"Sorry , i did not make advanced function"<<endl;
   else
   {
      int p1=1,p2=2,p3=3;
      for(int i=2; i<argc;i++)
          ran.push_back(*argv[i]);
      seed = trans(ran);
      pthread_t thr[4];
      pthread_mutex_init(&lock , NULL);
      pthread_create (&thr[0],NULL,dispatcher,(void*)"dispatcher");
      pthread_create (&thr[1],NULL,producer,&p1);
      pthread_create (&thr[2],NULL,producer,&p2);
      pthread_create (&thr[3],NULL,producer,&p3);
      pthread_join(thr[0] , NULL);
      pthread_join(thr[1] , NULL);
      pthread_join(thr[2] , NULL);
      pthread_join(thr[3] , NULL);
      
      cout<<endl<<"Dispatcher has prepared "<<dis_item[0]<<" [Propeller] module accessories"<<endl;
      cout<<"Dispatcher has prepared "<<dis_item[1]<<" [Battery] module accessories"<<endl;
      cout<<"Dispatcher has prepared "<<dis_item[2]<<" [Aircraft] module accessories"<<endl<<endl;
      
      sort(pdc , pdc+3, compare);
      for(int i=0;i<3;i++)
         cout<<pdc[i].pi<<pdc[i].num<<" aerial cameras"<<endl;

   }
}
