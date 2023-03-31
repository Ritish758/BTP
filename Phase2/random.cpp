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


int mx=2001;
int batch=3;


int rand_proc(int m){
  int x=rand();
  x=x%m;
  if(x<0){x=x+m;}
  x++;
  return x;
}

void random_schdule(vector<Task>& tasks, int m){

 int t=0;
 int i=0;
 
 vector<int> utilisation(m+1,0);
 vector<vector<int>> util_matrix;
 vector<int> time(m+1,0);
 
 vector<Task> intask;
 
 int drophard=0;
 int dropsoft=0;
 int eu_dis_hard = 0;
 int eu_dis_soft = 0;
 double eusqrt_dis_hard = 0.0;
 double eusqrt_dis_soft = 0.0;
 int c1=0;
 double util_cost = 0.2 * (double)m;
 
 queue<Task> processor[m+1];
 
 while(t<=mx){
 
  // cout<<"t= "<<t<<"\n";
 
    while(i<tasks.size() && tasks[i].arrival_time==t){intask.push_back(tasks[i]); i++;}
    
    for(int j=1; j<=m; j++){
      if(processor[j].size()==0){continue;}
      Task &x = processor[j].front();
      
      if(x.start_time==-1){
         x.start_time=t;
         x.end_time=t+x.process_time;
         //cout<<"Task "<<x.tid<<" started at processor "<<j<<"\n";
         utilisation[j]=1;
      }
      else if(x.end_time==t){
        //cout<<"Task "<<x.tid<<" compeleted at processos "<<j<<"\n";
        c1++;
        utilisation[j]=0;
        processor[j].pop();
        j--;
      }
      
    }
    
    if(t%batch==0 && intask.size()!=0){
    
      vector<Task> not_sch;
       
       for(auto x: intask){
          
          if(t+1+x.process_time > x.deadline){
            if(x.flag==0){drophard++;} //cout<<"Task "<<x.tid<<" Dropped\n"; }
            else{dropsoft++;} // cout<<"Task "<<x.tid<<" Dropped\n";}
            continue;
          }
            int proc=rand_proc(m);
            
            if(max(t+1,time[proc]) + x.process_time <= x.deadline){
                 processor[proc].push(x);
                 if(time[proc]==0 || time[proc]<=t){time[proc]=t+1+x.process_time;}
                 else{time[proc]+=x.process_time;}
               //  cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<"\n";
                    if(x.flag==0){
           		eu_dis_hard += eu_calc_dis(x.x, x.y,proc);
           		eusqrt_dis_hard += (double)(sqrtf(eu_calc_dis(x.x, x.y,proc)));
          
         	      }
         	      else{
           		eu_dis_soft += eu_calc_dis(x.x, x.y, proc);
           		eusqrt_dis_soft += (double)(sqrtf(eu_calc_dis(x.x, x.y,proc)));
         	      }                 
            }
            else{
               not_sch.push_back(x);
            
            }
          }
       intask=not_sch;
    
    }
    t++;
    
    util_matrix.push_back(utilisation);
 }
 
 
 
// cout<<"Utilization Matrix for each time slot"<<"\n";
  
  for(int i=0; i<util_matrix.size(); i++){
   // cout<<"Time =" << i<<" ";
    for(int j=1; j<util_matrix[i].size(); j++){
     // cout<<util_matrix[i][j]<<" ";
      util_cost += 0.8* (util_matrix[i][j]) * (util_matrix[i][j]) *(util_matrix[i][j]);
    }
   // cout<<"\n";
    }
  
  
//  cout<<"Total Hard Task dropped - "<<drophard<<"\n";
//   cout<<"Total Soft Task dropped - "<<dropsoft<<"\n";
//   cout<<"Distance used by Hard Tasks - "<<eu_dis_hard<<"  "<<eusqrt_dis_hard<<"\n";
//   cout<<"Distance used by Soft Tasks - "<<eu_dis_soft<<"  "<<eusqrt_dis_soft<<"\n";
//   cout<<"Utilisation Cost - "<<fixed<<setprecision(5)<<util_cost<<"\n";
//   cout<<"Number of completed Tasks - "<<c1<<"\n";
  
//   cout<<"\n\n\n";
  
  
  double final_cost_drop = ((double)drophard*0.9 + (double)dropsoft*0.1)/(double)(c1+drophard+dropsoft);
  double final_cost_dis = ((double)(eusqrt_dis_hard + eusqrt_dis_soft) / (double)(200.00 * (double)(sqrtf(2.00)))) /(double)(c1+drophard+dropsoft);
  double final_cost_energy = ((util_cost)/(double)m)/(double)(2000);
  
  // cout<<"Cost of dropping - "<<final_cost_drop<<"\n";
  // cout<<"Cost of distance - "<<final_cost_dis<<"\n";
  // cout<<"Cost of energy - "<<final_cost_energy<<"\n";
  
  double final_cost = (final_cost_drop + final_cost_dis + final_cost_energy)/(3.000);

// cout<<"Final Cost = "<<final_cost<<"\n";
cout<<final_cost_drop<<","<<final_cost_dis<<","<<final_cost_energy<<"\n";      
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
   srand(time(NULL));
   
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
   
   random_schdule(tasks,m);
   
   
   return 0;

}
