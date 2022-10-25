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

bool comp(Task &a, Task &b){
    if(a.flag==b.flag){
       return a.deadline < b.deadline;
    }
    
    return a.flag<b.flag;

}

int bestmatch(vector<double> &util, double req, vector<int> &time, Task t, int cur_time){

 int bestproc=-1;
 
 double ma=0.0;
 
 double ut=1.0 - req;
 
 //cout<<ut <<"\n";
 
 for(int i=1; i<util.size(); i++){
     if(max(cur_time,time[i]) + t.process_time + 1 <= t.deadline){
     if((ut-util[i]) >= 0.000001){
        
        if((ma - util[i])<=0.000001){
            ma=util[i];
            bestproc=i;
        
        }
     }
     }
 }
 
 return bestproc;
}



int mx=2001;
int batch=3;

void schedule_greedy1(vector<Task>& tasks, int m){
   
   vector<double> utilization(m+1,0);
   vector<int> time(m+1,0);
   
   vector<vector<double>> util_matrix;
   
   int t=0;
   int i=0;
   
   int drophard=0;
   int dropsoft=0;
   
   vector<Task> intask;
   
   queue<Task> processors[m+1];
   
   while(t<=mx){
   
   cout<<"t="<<t<<"\n";
   
   if(i<tasks.size()){
     while(t==tasks[i].arrival_time && i<tasks.size()){intask.push_back(tasks[i]); i++;}
   }
   
   for(int j=1; j<=m; j++){
      if(processors[j].size()==0){continue;}
      
      Task &x=processors[j].front();
      if(x.start_time==-1){
        x.start_time=t;
        x.end_time=t+x.process_time;
        cout<<"Task "<<x.tid<<" started at processor "<<j<<"\n";
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
   
   if(t%batch==0){
     sort(intask.begin(),intask.end(),comp);
     vector<Task> not_Sch;
     for(auto x: intask){
      if(x.deadline<=t || x.process_time + t >x.deadline){
         if(x.flag==0){drophard++;}
         else{dropsoft++;}
         cout << "Task "<<x.tid<<" Dropped\n";
         continue;
      }
      double util = (double)(x.process_time)/(double)(x.deadline-x.arrival_time);
      int proc=bestmatch(utilization, util, time, x, t);
      //cout<< x.tid <<" "<< proc<<" "<<util<<"\n";
      if(proc!=-1){
        processors[proc].push(x);
        utilization[proc] += util;
        cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<"\n";
        if(time[proc]==0){time[proc] = t+1;}
        time[proc] += x.process_time;
      }
      else{
       not_Sch.push_back(x);
      }
     }
     intask=not_Sch;
   }
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
   
   int m;
   fin>>m;
   
   while(fin.good()){
      vector<int> x(5);
      fin>>x[0]>>x[1]>>x[2]>>x[3]>>x[4];
      
      Task t(x[0],x[2],x[1],x[4],x[3]);
      
      tasks.push_back(t);
   }
   
   schedule_greedy1(tasks,m);
   
   return 0;

}
