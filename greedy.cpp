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

int bestmatch(vector<int>& time, int cur_time, Task t){

   int bestproc=-1;
   
   int larg_time=0;
   
   for(int i=1; i<time.size(); i++){
      if(max(time[i],cur_time+1) + t.process_time <= t.deadline){
          if(larg_time <= max(time[i],cur_time+1)){
            bestproc=i;
            larg_time=max(time[i],cur_time+1);
          }
      }
   
   }
   
   return bestproc;
}

int bestprocdrop(vector<vector<Task>>& proc, Task t, vector<int>& time, int cur_time){

 int bestproc=-1;
 
 int no=1e6;
 
 for(int i=1; i<proc.size(); i++){
     vector<int> temp;
     
     for(auto x: proc[i]){
       if(x.flag==0){continue;}
       if(x.start_time==-1){temp.push_back(x.process_time);}
       else{
         temp.push_back(x.end_time-cur_time-1);
       }
     }
     
     sort(temp.begin(),temp.end());
     
     int val=0;
     int nu=0;
     
     for(int j=temp.size()-1; j>=0; j--){
         val=val+temp[j];
         nu++;
         if(time[i]-val + t.process_time <= t.deadline){
            if(no>nu){
               bestproc=i;
               no=nu;
               break;
            }  
         }
     }
 }
 
 
 return bestproc;
}



int mx=2001;
int batch=3;

void schedule_greedy1(vector<Task>& tasks, int m){
   
   vector<int> time(m+1,0);
   
   int t=0;
   int i=0;
   
   int drophard=0;
   int dropsoft=0;
   
   vector<Task> intask;
   
   vector<vector<Task>> processors(m+1);
   
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
        cout<<"Task "<<x.tid<<" completed at processor "<<j<<"\n";
        processors[j].erase(processors[j].begin());
        j--;
        continue;
      }
   } 
   
   if(t%batch==0 && intask.size()>0){
     sort(intask.begin(),intask.end(),comp);
     vector<Task> not_Sch;
     for(auto x: intask){
      if(x.deadline<=t || x.process_time + t >x.deadline){
         if(x.flag==0){drophard++;}
         else{dropsoft++;}
         cout << "Task "<<x.tid<<" Dropped\n";
         continue;
      }
      int proc=bestmatch(time,t,x);
      //cout<< x.tid <<" "<< proc<<" "<<util<<"\n";
      if(proc!=-1){
        processors[proc].push_back(x);
        cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<"\n";
        if(time[proc]==0 || time[proc]<=t){time[proc] = t+1;}
        time[proc] += x.process_time;
      }
      else{
       if(x.flag==0){
          int proc=bestprocdrop(processors,x,time,t);
          
          if(proc==-1){not_Sch.push_back(x); continue;}
          
          vector<pair<int,int>> temp;
          vector<Task> tempor;
          for(int j=0; j<processors[proc].size(); j++){
             if(processors[proc][j].flag==0){continue;}
             if(processors[proc][j].start_time==-1){temp.push_back({processors[proc][j].process_time,j});}
             else{
                temp.push_back({processors[proc][j].end_time - t-1 , j});
             }
          }
          
          sort(temp.begin(),temp.end());
          
          vector<int> droped;
          int val=0;
          
          for(int k=temp.size()-1; k>=0; k--){
             val=val+temp[k].first;
             droped.push_back(temp[k].second);
             if(time[proc]-val + x.process_time <= x.deadline){
               break;
             }
          }
          
          sort(droped.begin(),droped.end());
          
          int k=0;
                    
          for(int j=0; j<processors[proc].size(); j++){
             if(k<droped.size() && j==droped[k]){cout<<"Task "<<x.tid<<" Drooped soft-hard\n";continue;}
             tempor.push_back(processors[proc][j]);
          }
          tempor.push_back(x);
          continue;
       } 
       not_Sch.push_back(x);
      }
     }
     intask=not_Sch;
   }
   t++;
   
 }
   
   
  cout<<"Total Hard Task dropped - "<<drophard<<"\n";
  cout<<"Total Soft Task dropped - "<<dropsoft<<"\n";
  
  cout<<"Utilization Matrix for each time slot"<<"\n";
  
  

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
