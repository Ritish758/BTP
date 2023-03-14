#include<bits/stdc++.h>

using namespace std;


map<int,pair<int,int>> base_station;

int eu_calc_dis(int x, int y, int id){
    return abs(x-base_station[id].first)*abs(x-base_station[id].first) + abs(y-base_station[id].second)*abs(y-base_station[id].second);
}


struct Task{
   int arrival_time;
   int deadline;
   int process_time;
   int tid;
   bool flag;
   int start_time;
   int end_time;
   double utilisation;
   int x;
   int y;
   
   Task(int a, int d,int pr, int id, bool fl, int x,int y){
     arrival_time=a;
     deadline=d;
     process_time=pr;
     tid=id;
     flag=fl;
     start_time=-1;
     end_time=-1;
     utilisation=0.0;
     x=x;
     y=y;
     //changed_process_time=pr;
   }
};

bool comp(Task &a, Task &b){
    if(a.flag==b.flag){
       return a.deadline < b.deadline;
    }
    
    return a.flag<b.flag;

}

bool comp1(Task& a, Task& b){
     return (a.utilisation > b.utilisation);
}


int bestmatch(int cur_time, Task &t, vector<vector<double>>& util){

double ut = (double)(t.process_time)/(double)(t.deadline - cur_time -1);

int start_time = cur_time+1;
int end_time = t.deadline;
     double val = 1.000000 - ut;

      double close=1.0;
      int proc=-1;
      
      for(int j=1; j<util.size(); j++){
         bool ok=true;
         double ma=0.0;
         for(int i=start_time; i<=end_time; i++){
             if((util[j][i] - val) <= -0.000001){
                ma=max(ma,util[j][i]);
             }
             else{
               ok=false;
               break;
             } 
         }
         
         if(ok){
            cout<<close<<" "<<ma<<"normal \n";
             if((ma - close) <= -0.000001){
               proc=j;
               close=ma;
             }
             else if(proc==-1){proc=j; close=ma;}
         }
      
      }
      
      if(proc!=-1){
        t.start_time=start_time;
        t.end_time=end_time;
        t.utilisation=ut;
        return proc;
      } 
      
      return -1;

}

int bestprocdrop(vector<vector<Task>>& proc, Task t,vector<vector<double>> util, int cur_time,set<int> &dr){


int start_time=cur_time+1;
int end_time=t.deadline;

double requtil = (double)(t.process_time)/(double)(end_time-start_time);

cout<<requtil<<"\n";

int no=1000000;
int bestproc=-1;



for(int j=1; j<util.size(); j++){

    set<int> s;
    bool ok=true;
  
     
     for(int i=start_time; i<=end_time; i++){
          if((requtil + util[j][i]) >= 1.0000001){
             vector<Task> temp;
              
              for(auto x: proc[j]){
                if(x.flag==0){continue;}
                if(!(i>=x.start_time && i<=x.end_time)){continue;}
                 temp.push_back(x);
              }
              
              if(temp.size()==0){ok=false; break;}
              sort(temp.begin(),temp.end(),comp1);
              
              double val=util[j][i];
              bool ok1=false;
              int no1=0;
              
              for(auto x: temp){
                  val=val-x.utilisation;
                  no1++;
                  if((val+requtil)<=1.000000){
                       ok1=true;
                        break;
                  }
              
              }
              
              if(ok1==true){
                  
                for(int q=0; q<no1; q++){
                  s.insert(temp[q].tid);
                }
              }
              else{ok=false; break;}
          }
     }
     
     if(ok==true){  
         if(no<s.size()){continue;}
         else{
            no=s.size();
            bestproc=j;
            dr=s;
         }
     }
}


return bestproc;

}



int mx=2001;
int batch=3;

void schedule_greedy1(vector<Task>& tasks, int m){


 int t=0;
 int i=0;
 
 vector<Task> intask;
 
 vector<vector<double>> util(m+1,vector<double>(mx+5,0.0));
 
 vector<vector<double>> soft(m+1,vector<double>(mx+5,0.0));
 
 vector<vector<Task>> processor(m+1);
 
 int drophard=0;
 int dropsoft=0;
 int dis_hard = 0;
 int dis_soft = 0;
 double util_cost = 0.2*(double)m;
 
 
 int c1=0;
 
 while(t<=mx){
 
 cout<<"t="<<t<<"\n";
 
 while(i<tasks.size() && tasks[i].arrival_time==t){intask.push_back(tasks[i]); i++; }
 
 for(int k=1; k<=m; k++){
    if(processor[k].size()==0){continue;}
    
    vector<Task> temp;
    
    for(int j=0; j<processor[k].size(); j++){
       if(processor[k][j].start_time==t){
          cout<<"Task "<<processor[k][j].tid<<" started at processor "<<k<<"\n";
       }
       else if(processor[k][j].end_time==t){
        cout<<"Task "<<processor[k][j].tid<<" completed at processor "<<k<<"\n";
        c1++;
        continue;
       }
       temp.push_back(processor[k][j]);
    }
    processor[k]=temp;  
 }
 
 if(t%batch==0 && intask.size()!=0){
   sort(intask.begin(),intask.end(),comp);
   vector<Task> not_sch;
   
   for(auto x: intask){
      if(x.process_time + t+1 > x.deadline){
         if(x.flag==0){drophard++;}
         else{dropsoft++;}
         cout<<"Task "<<x.tid<<" Dropped\n";
         continue;
      }
      double utilisation = (x.process_time)/(x.deadline - t-1);
      x.utilisation = utilisation;
      int sch_time = 0;
      int proc = bestmatch(t,x,util);
      if(proc!=-1){
         cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<" with utilisation "<<x.utilisation<<"\n";
         processor[proc].push_back(x);
         for(int j=x.start_time; j<=x.end_time; j++){
             util[proc][j] += x.utilisation;
         }
         if(x.flag==0){
           dis_hard += eu_calc_dis(x.x,x.y,proc);
         }
         else{
           dis_soft += eu_calc_dis(x.x,x.y,proc);
         }
      }
      else{
        if(x.flag==0){
           set<int> dr;
           proc = bestprocdrop(processor,x,util,t,dr);
           //cout<<proc<<"\n";
           if(proc==-1){
              not_sch.push_back(x);
           }
           else{
              vector<Task> temp;
              for(int tsk=0; tsk<processor[proc].size(); tsk++){
                  if(dr.find(processor[proc][tsk].tid)!=dr.end()){
                     cout<<"Task "<<processor[proc][tsk].tid<<" dropped hard-soft\n";
                     dropsoft++;
                     for(int tm=t; tm<=processor[proc][tsk].end_time; tm++){
                         util[proc][tm]-= processor[proc][tsk].utilisation;
                     }
                     continue;
                  }
                  temp.push_back(processor[proc][tsk]);
              }
              x.utilisation = (double)(x.process_time)/(double)(x.deadline - t-1);
              processor[proc]=temp;
              for(int tm=t+1; tm<=x.deadline; tm++){
                   util[proc][tm]+=x.utilisation;
              }
              cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<" with utilisation "<<x.utilisation<<"\n";
              x.start_time=t+1;
              x.end_time=x.deadline;
              processor[proc].push_back(x);
              dis_hard += eu_calc_dis(x.x,x.y,proc);
           }
        }
        else{
           not_sch.push_back(x);
        }
      }
   }
   intask=not_sch;
 
 }
 t++;
 
 for(int j=1; j<util.size(); j++){
   cout<<util[j][t-1]<<" ";
   util_cost += 0.8 * (util[j][t-1]) *(util[j][t-1]) * (util[j][t-1]);
 }
 cout<<"\n";
 
 }
 
  cout<<"Total Hard Task dropped - "<<drophard<<"\n";
  cout<<"Total Soft Task dropped - "<<dropsoft<<"\n";
  cout<<"Hard Task distance - "<<dis_hard<<"\n";
  cout<<"Soft Task distance - "<<dis_soft<<"\n";
  cout<<"Cost of utilisation - "<<fixed<<setprecision(5)<<util_cost<<"\n";
  
  cout<<c1<<"\n";
  
  cout<<"\n\n\n";
  
  cout<<drophard*100+dropsoft<<"\n";
  double final_cost = ((double)drophard*0.9 + (double)dropsoft*0.1)/(double)(c1+drophard+dropsoft);
  final_cost += ((double)(dis_hard + dis_soft) / (double)(200*2*200)) /(double)(c1+drophard+dropsoft);
  final_cost += ((util_cost)/(double)m)/(double)(2000);
  
  cout<<"Final Cost = "<<final_cost<<"\n";
  

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
   int ct=1;
   while(ct<=m){
     int x,y;
     fin>>x>>y;
     base_station[ct] = {x,y};
     ct++;
   }
   
   while(fin.good()){
      vector<int> x(7);
      fin>>x[0]>>x[1]>>x[2]>>x[3]>>x[4]>>x[5]>>x[6];
      Task t(x[0],x[2],x[1],x[6],x[3],x[4],x[5]);
      tasks.push_back(t);
   }
   
   schedule_greedy1(tasks,m);
   
   return 0;

}
