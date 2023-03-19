#include<bits/stdc++.h>

using namespace std;

map<int,pair<int,int>> base_station;

double max_util = 0.9;   // can tune max util of a base station

int dis_hop=1000000;   // can tune max distance hop for a soft task please update this in int_main


int eu_calc_dis(int x, int y, int id){
    return abs(x-base_station[id].first)*abs(x-base_station[id].first) + abs(y-base_station[id].second)*abs(y-base_station[id].second);
}

set<int> proc_req(int x, int y){
   vector<pair<int,int>> check;
   for(auto it: base_station){
       check.push_back({abs(x-it.second.first)*abs(x-it.second.first) + abs(y-it.second.second)*abs(y-it.second.second), it.first});
   } 
   sort(check.begin(),check.end());
   set<int> ans;
   for(int i=0; i<check.size(); i++){
      if(check[0].first == check[i].first){ans.insert(check[i].second);}
   }
   return ans;
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
   
   Task(int a, int d,int pr, int id, bool fl, int x, int y){
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


int bestmatch(int cur_time, Task &t, vector<vector<double>>& util,set<int> req_proc){

double ut = (double)(t.process_time)/(double)(t.deadline - cur_time -1);

int start_time = cur_time+1;
int end_time = t.deadline;

if(t.flag==0 && (ut - 0.7)<= -0.000001){
      int process_time = (t.process_time*10)/7;
      if((t.process_time*10)%7!=0){process_time++;}
      end_time = start_time + process_time;
      
      double close=0.0;
      int proc=-1;
      
      for(int j=1; j<util.size(); j++){
      	 if(req_proc.find(j)==req_proc.end()){continue;}
         bool ok=true;
         double ma=0.0;
         for(int i=start_time; i<=end_time; i++){
             if((util[j][i] - (max_util - 0.7)) <= -0.000001){
                ma=max(ma,util[j][i]);
             }
             else{
               ok=false;
               break;
             } 
         }
         
         if(ok){
             //cout<<close<<" "<<ma<<"  0.7\n";
             if((close - ma) <= -0.000001){
               proc=j;
               close=ma;
             }
             else if(proc==-1){proc=j; close=ma;}
           
             if((close - ma) <= 0.0000001){
               if((eu_calc_dis(t.x,t.y,j)) < eu_calc_dis(t.x,t.y,proc)){
                proc=j;
               }
             }
         }
      
      }
      
      if(proc!=-1){
        t.start_time=start_time;
        t.end_time=end_time;
        t.utilisation=0.7;
        return proc;
      }   
}

     double val = max_util- ut;

      double close=0.0;
      int proc=-1;
      
      for(int j=1; j<util.size(); j++){
         if(req_proc.find(j)==req_proc.end()){continue;}
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
           // cout<<close<<" "<<ma<<"normal \n";
             if((close - ma) <= -0.000001){
               proc=j;
               close=ma;
             }
             else if(proc==-1){proc=j; close=ma;}
             
             if((close - ma) <= 0.0000001){
               if((eu_calc_dis(t.x,t.y,j)) < eu_calc_dis(t.x,t.y,proc)){
                proc=j;
               }
             }
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

int bestprocdrop(vector<vector<Task>>& proc, Task t,vector<vector<double>> util, int cur_time,set<int> &dr, set<int> req_proc){


int start_time=cur_time+1;
int end_time=t.deadline;

double requtil = (double)(t.process_time)/(double)(end_time-start_time);

//cout<<requtil<<"\n";

int no=1000000;
int bestproc=-1;



for(int j=1; j<util.size(); j++){
    if(req_proc.find(j)==req_proc.end()){continue;}

    set<int> s;
    bool ok=true;
  
     
     for(int i=start_time; i<=end_time; i++){
          if((requtil + util[j][i]) >= max_util){
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
                  if((val+requtil)<=max_util){
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
 int c1=0;
 int eu_dis_hard = 0;
 int eu_dis_soft = 0;
 double eusqrt_dis_hard = 0.0;
 double eusqrt_dis_soft = 0.0;
 
 double util_cost = 0.2 * (double)m;
 
 while(t<=mx){
 
// cout<<"t="<<t<<"\n";
 
 while(i<tasks.size() && tasks[i].arrival_time==t){intask.push_back(tasks[i]); i++; }
 
 for(int k=1; k<=m; k++){
    if(processor[k].size()==0){continue;}
    
    vector<Task> temp;
    
    for(int j=0; j<processor[k].size(); j++){
       if(processor[k][j].start_time==t){
          //cout<<"Task "<<processor[k][j].tid<<" started at processor "<<k<<"\n";
       }
       else if(processor[k][j].end_time==t){
       // cout<<"Task "<<processor[k][j].tid<<" completed at processor "<<k<<"\n";
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
        // cout<<"Task "<<x.tid<<" Dropped\n";
         continue;
      }
      double utilisation = (x.process_time)/(x.deadline - t-1);
      x.utilisation = utilisation;
      int sch_time = 0;
      set<int> req_proc = proc_req(x.x, x.y);
      int proc = bestmatch(t,x,util,req_proc);
      if(proc!=-1){
         bool run = true;
      	 if(x.flag==0){
           eu_dis_hard += eu_calc_dis(x.x, x.y,proc);
           eusqrt_dis_hard += (double)(sqrtf(eu_calc_dis(x.x, x.y,proc)));
          
         }
         else{
           eu_dis_soft += eu_calc_dis(x.x, x.y, proc);
           eusqrt_dis_soft += (double)(sqrtf(eu_calc_dis(x.x, x.y,proc)));
         }
         if(run == true){
        // cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<" with utilisation "<<x.utilisation<<"\n";
         processor[proc].push_back(x);
         for(int j=x.start_time; j<=x.end_time; j++){
             util[proc][j] += x.utilisation;
         }
         }
      }
      else{
        if(x.flag==0){
           set<int> dr;
           proc = bestprocdrop(processor,x,util,t,dr,req_proc);
           //cout<<proc<<"\n";
           if(proc==-1){
              not_sch.push_back(x);
           }
           else{
              vector<Task> temp;
              for(int tsk=0; tsk<processor[proc].size(); tsk++){
                  if(dr.find(processor[proc][tsk].tid)!=dr.end()){
                   //  cout<<"Task "<<processor[proc][tsk].tid<<" dropped hard-soft\n";
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
             // cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<" with utilisation "<<x.utilisation<<"\n";
              x.start_time=t+1;
              x.end_time=x.deadline;
              processor[proc].push_back(x);
              eu_dis_hard += eu_calc_dis(x.x, x.y,proc);
              eusqrt_dis_hard += (double)(sqrtf(eu_calc_dis(x.x, x.y,proc)));
              
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
   //cout<<util[j][t-1]<<" ";
   util_cost += 0.8* (util[j][t-1]) * (util[j][t-1]) *(util[j][t-1]);
 }
// cout<<"\n";
 
 }
 
  cout<<"Total Hard Task dropped - "<<drophard<<"\n";
  cout<<"Total Soft Task dropped - "<<dropsoft<<"\n";
  cout<<"Distance used by Hard Tasks - "<<eu_dis_hard<<"  "<<eusqrt_dis_hard<<"\n";
  cout<<"Distance used by Soft Tasks - "<<eu_dis_soft<<"  "<<eusqrt_dis_soft<<"\n";
  cout<<"Utilisation Cost - "<<fixed<<setprecision(5)<<util_cost<<"\n";
  cout<<"Number of completed Tasks - "<<c1<<"\n";
  
  cout<<"\n\n\n";
  
  
  double final_cost_drop = ((double)drophard*0.9 + (double)dropsoft*0.1)/(double)(c1+drophard+dropsoft);
  double final_cost_dis = ((double)(eusqrt_dis_hard + eusqrt_dis_soft) / (double)(200.00 * (double)(sqrtf(2.00)))) /(double)(c1+drophard+dropsoft);
  double final_cost_energy = ((util_cost)/(double)m)/(double)(2000);
  
  cout<<"Cost of dropping - "<<final_cost_drop<<"\n";
  cout<<"Cost of distance - "<<final_cost_dis<<"\n";
  cout<<"Cost of energy - "<<final_cost_energy<<"\n";
  
  double final_cost = (final_cost_drop + final_cost_dis + final_cost_energy)/(3.000);
  
  cout<<"Final Cost = "<<final_cost<<"\n";

}







int main(int argc, char* argv[]){

   if(argc<2){
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
