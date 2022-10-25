#include<bits/stdc++.h>

using namespace std;



struct Task{
   int arrival_time;
   int deadline;
   int process_time;
   int tid;
   bool flag;
   int start_time;
   int end_time;
   
   Task(int a, int d,int pr, int id, bool fl){
     arrival_time=a;
     deadline=d;
     process_time=pr;
     tid=id;
     flag=fl;
     start_time=-1;
     end_time=-1;
   }
};


int rand_proc(int m){
  int x;
  x=rand();
  
  x=x%m;
  if(x<0){x=x+m;}
  return x;
}


int find_rand_proc(vector<double>& util, Task t, vector<int>& time, int cur_time){

vector<int> ans;

double req = (double)t.process_time/(double)(t.deadline-t.arrival_time);

double comp = 1.0 - req;

for(int i=1; i<util.size(); i++){
     if(max(cur_time,time[i]) + t.process_time + 1 <= t.deadline){
        if((comp - util[i]) >= 0.000001){
            ans.push_back(i);
        }
     }
}


if(ans.size()==0){return -1;}

int val=rand_proc(ans.size());

return ans[val];
}

int mx=2001;

void random_schdule(vector<Task>& tasks, int m){

     vector<double> utilization(m+1,0);
     vector<int> time(m+1,0);
     
     queue<Task> processors[m+1];
     
     vector<vector<double>> util_matrix;
     
     int drophard=0;
     int dropsoft=0;
     
     int t=0;
     int i=0;
     
     vector<Task> intask;
     
     while(t<=mx){
      cout<<"t = "<<t<<"\n";
      
      if(i<tasks.size()){
         while(t==tasks[i].arrival_time && i<tasks.size()){intask.push_back(tasks[i]); i++;}
      }
      for(int j=1; j<=m; j++){
        if(processors[j].size()==0){continue;}
        Task &x = processors[j].front();
        
        if(x.start_time==-1){
          x.start_time=t;
          x.end_time=t+x.process_time;
          cout <<"Task "<<x.tid<<" started at processor "<<j<<"\n";
        }
        else if(x.end_time==t){
          double util = (double)x.process_time/(double)(x.deadline-x.arrival_time);
          utilization[j] -= util;
          cout<<"Task "<<x.tid<<" completed at processor "<<j<<"\n";
          processors[j].pop();
          j--;
          continue;
        }
      }
      
      vector<Task> not_sch;
      
      for(auto x: intask){
         if(x.deadline <=t || x.process_time + t> x.deadline){
            if(x.flag==0){drophard++;}
            else{dropsoft++;}
            cout<<"Task "<<x.tid<<" Dropped\n";
            continue;
         }
         double util = (double)(x.process_time)/(double)(x.deadline-x.arrival_time);
         int proc=find_rand_proc(utilization,x,time,t);
         
         if(proc !=-1){
           processors[proc].push(x);
           utilization[proc]+=util;
           cout<<"Task "<<x.tid<<" assigned to processor " << proc<<"\n";
           if(time[proc]==0){
             time[proc]=t+1;
           }
           time[proc] +=x.process_time;
         }
         else{
           not_sch.push_back(x);
         }
      }
      intask=not_sch;
      t++;
      util_matrix.push_back(utilization);
   }
   
   cout<<"Total Hard Task dropped - "<<drophard<<"\n";
   cout<<"Total Soft Task dropped - "<<dropsoft<<"\n";
  
   cout<<"Utilization Matrix for each time slot"<<"\n";
  
   for(int i=0; i<util_matrix.size(); i++){
    cout<<"Time "<<i<<" ";
     for(int j=1; j<=m; j++){
        cout<<util_matrix[i][j] <<" ";
     }
     cout<<"\n";
   }   
   
}








int main(int argc, char* argv[]){

   if(argc!=2){
    cout<<"Please provide correct arguments\n";
    return 0;
   }
   
   string filename=argv[1];

   vector<Task> tasks;
   
   ifstream fin;
   
   fin.open(filename, ios::in);
   
   srand(time(0));
   
   int m;
   fin>>m;
   
   while(fin.good()){
      vector<int> x(5);
      fin>>x[0]>>x[1]>>x[2]>>x[3]>>x[4];
      
      Task t(x[0],x[2],x[1],x[4],x[3]);
      
      tasks.push_back(t);
   }
   
   random_schdule(tasks,m);
   
   return 0;

}
