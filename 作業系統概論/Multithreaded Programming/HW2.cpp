#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <pthread.h>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <cmath>
#include <iomanip>
#include<ctime>
using namespace std;
void judge(char ele, int& no)
{
    if (ele >= 65 && ele <= 90)
        no = 0;
    else if (ele >= 97 && ele <= 122)
        no = 0;
    else if (ele >= 48 && ele <= 57)
        no = 2;
    else
        no = 1;
}
//=======determine whether it is a letter========//

struct cos
{
   int value;
   string ele1;
   string ele2;
};
//=====create a struct to save the calculate value of cosine======//

int list_size = 0;
double total_avg[52];
int file_size = 0, ele_size[1000] = { 0 };
string ID[52];
vector<vector<string>> str(52);
vector<string> word_list;
vector<vector<int>> frequen;
//==================Golbal variable=================//

struct cos cos_list[2700];

pid_t tid;   //create tid

void* child(void*arg)
{
   double Start = clock();
   tid = gettid();  //get tid//
   usleep(1);
   int n = *(int *)arg;  //current file//
   cout<<"[TID="<<tid<<"] DocID:"<<ID[n]<<" [";  
   for(long unsigned int i=0; i<frequen[n].size()-1; i++)
      cout<<frequen[n][i]<<",";
   cout<<frequen[n][frequen[n].size()-1]<<"]"<<endl;
   
   int cos_list_count = list_size , in_list = 0 ,val_count = 0;  //cos_list_count:convenient insert the data to cos_list
   double val[52] , avg = 0;
   for(int i=0; i<file_size; i++)
   {
      double sum_up = 0,abs_vs = 0,abs_vx = 0 , ans = 0;
      if(i == n)
         continue;
      for(int j=0; j<list_size; j++)
      {    
          //====check if the element to be calculated is in cos_list,yes:ans = value of cos_list no:calculate
         if((ID[n] == cos_list[j].ele1 && ID[i] == cos_list[j].ele2) || (ID[i] == cos_list[j].ele2 && ID[n] == cos_list[j].ele1))
         {
            ans = cos_list[j].value;
            in_list = 1;
         }
      }
      if(in_list == 0)  //not in cos_list,calculate
      {  
         for(long unsigned int k=0; k<frequen[i].size(); k++)
         {
            sum_up = sum_up + frequen[n][k]*frequen[i][k];
            abs_vs = abs_vs+pow(frequen[n][k],2);
            abs_vx = abs_vx+pow(frequen[i][k],2);
         }
         ans = sum_up/(sqrt(abs_vs)*sqrt(abs_vx));
         cos_list[cos_list_count].value = ans;
         cos_list[cos_list_count].ele1 = ID[n];
         cos_list[cos_list_count].ele2 = ID[i];     //record value and element of calculate in cos_list
         list_size++;
         cos_list_count++;
      }
      cout<<fixed<<setprecision(4);
      cout<<"[TID="<<tid<<"] cosine("<<ID[n]<<","<<ID[i]<<")=";
      cout<<ans<<endl;
      
      val[val_count] = ans;
      val_count++;
   }
   
   for(int i=0; i<val_count; i++)  //calculate the average of each value
      avg = avg+val[i];
   avg = avg/val_count;
   cout<<"[TID="<<tid<<"] AVG_cosine: "<<avg<<endl;
   total_avg[n] = avg;
   double End = clock();
   cout<<fixed<<setprecision(0);
   cout<<"[TID="<<tid<<"] CPU time: "<<End-Start<<"ms"<<endl;
   return NULL;
}


int main()
{
    cout<<"Please enter the data file name(include filename extension .txt): ";
    string file;
    cin>>file;
    double Start = clock();
    ifstream file_in;
    file_in.open(file);
    string input, data[52];
    //=========================get data========================//
    
    bool file_type = false;  //=====view data as ID or file content======//
    int i = 0, ele_s = 0;    //
    while (!file_in.eof())
    {
        getline(file_in, input);
        if (file_type == false)  //=========save the ID in golbal variable ID[]==========//
        {
            if(input[input.size()-1] == ' ')
               input.pop_back();
            ID[i] = input;
            file_type = true;
            i++;
        }
        else
        {
            data[file_size] = input;  //=========save the content in golbal variable data[]==========//
            file_type = false;
            file_size++;
        }
    }
    //==========differentiate ID and content==========//
    
    for (int i = 0; i < file_size; i++)
    {
        int no = 0;
        for (long unsigned int j = 0; j < data[i].size(); j++)
        {
            no = 0;
            judge(data[i][j], no);
            if (no == 1)
            {
                data[i][j] = ' ';
            }
            else if (no == 2)
            {
                long unsigned int k = j;
                while (data[i][k] != ' ' && k != data[i].size() - 1)
                {
                    data[i][k] = ' ';
                    k++;
                }
                k = j;
                if (k != 0)
                {
                    k = j - 1;
                    while (data[i][k] != ' ' && k != 0)
                    {
                        data[i][k] = ' ';
                        k--;
                    }
                    data[i][j] = ' ';
                }
            }
        }
    }
    //==============if content have punctuation or letter of the word have nuber,let it is space(filter)=============//
    
    for (int i = 0; i < file_size; i++)
    {
        for (long unsigned int j = 0; j < data[i].size(); j++)
        {
            string str_tmp;
            if (data[i][j] != ' ')
            {
                long unsigned int ptr = j;
                while (data[i][ptr] != ' ' && ptr != data[i].size() - 1)
                {
                    str_tmp.push_back(data[i][ptr]);
                    ptr++;
                }
                if (ptr == data[i].size() - 1 && data[i][ptr] != ' ')
                    str_tmp.push_back(data[i][ptr]);
                str[i].push_back(str_tmp);
                j = ptr;
                ele_s++;
            }
        }
        ele_size[i] = ele_s;
        ele_s = 0;
    }
    //==============arrange the content and add it to str,str is two-dimensional string vector===========//
    //==============str[a] is content of each file=================//

    for (int i = 0; i < file_size; i++)
    {
        for (long unsigned int j = 0; j < str[i].size(); j++)
        {
            long unsigned int repeat = 0,list_times = 0;
            while (list_times != word_list.size() && word_list.size() != 0)
            {
                if (str[i][j] == word_list[list_times])
                {    
                    repeat = 1;
                    break;
                }
                list_times++;
            }
            if (repeat == 0)
            {
                if(str[i].size()!=0)
                  word_list.push_back(str[i][j]);
            }
        }
    }
   //=============word_list is all word appearing in all documents==============//
   
   for(int i=0; i<file_size; i++)
   {
      vector<int> f_tmp(word_list.size() , 0);
      for(long unsigned int j=0; j<str[i].size(); j++)
      {
         for(long unsigned int k=0; k<word_list.size(); k++)
         {
            if(str[i][j] == word_list[k])
               f_tmp[k]++;
         }
      }
      frequen.push_back(f_tmp);
   }
   //=============the frequency of each file's word appearing in the file=============//
   
   pthread_t threads;
   for(int i=0; i<file_size; i++)
   {
      int arg = i;
      pthread_create(&threads,NULL,child,&arg);  //create//
      usleep(1);   
      cout<<"[Main thread]: create TID:"<<tid<<",DocID:"<<ID[i]<<endl;  
      pthread_join(threads , NULL);  //end child_thread//
   }
   //========create the pthread========//
   
   cout<<fixed<<setprecision(4);
   double high = 0;
   int local;
   for(int i=0; i<file_size; i++)
   {
      if(total_avg[i]>high)
      {
         high = total_avg[i];
         local = i;
      }
   }
   cout<<"[Main thread] KeyDocID:"<<ID[local]<<" Highest Average Cosine: "<<high<<endl;
   cout<<fixed<<setprecision(0);
   double End = clock();
   cout<<"[Main thread] CPU time: "<<End-Start<<"ms"<<endl;
}
