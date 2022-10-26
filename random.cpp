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
 
 vector<double> utilisation(m+1,0);
 vector<int> time(m+1,0);
 
 vector<Task> intask;
 
 int drophard=0;
 int dropsoft=0;
 
 queue<Task> processor[m+1];
 
 while(t<=mx){
 
   cout<<"t= "<<t<<"\n";
 
    while(i<tasks.size() && tasks[i].arrival_time==t){intask.push_back(tasks[i]); i++;}
    
    for(int j=1; j<=m; j++){
      if(processor[j].size()==0){continue;}
      Task &x = processor[j].front();
      
      if(x.start_time==-1){
         x.start_time=t;
         x.end_time=t+x.process_time;
         cout<<"Task "<<x.tid<<" started at processor "<<j<<"\n";
         utilisation[j]=1;
      }
      else if(x.end_time==t){
        cout<<"Task "<<x.tid<<" compeleted at processos "<<j<<"\n";
        utilisation[j]=0;
        processor[j].pop();
        j--;
      }
      
    }
    
    if(t%batch==0 && intask.size()!=0){
    
      vector<Task> not_sch;
       
       for(auto x: intask){
          
          if(t+1+x.process_time > x.deadline){
            if(x.flag==0){drophard++; cout<<"Task "<<x.tid<<" Dropped\n"; }
            else{dropsoft++; cout<<"Task "<<x.tid<<" Dropped\n";}
            continue;
          }
       
          if(x.flag==0){
            int temp=0;
            
            while(temp<2*m){
               int proc=rand_proc(m);
               
               if(max(t+1,time[proc]) + x.process_time <= x.deadline){
                     processor[proc].push(x);
                     if(time[proc]==0 || time[proc]<=t){time[proc]=t+1+x.process_time;}
                     else{time[proc]+=x.process_time;}
                     break;
               }
               
               queue<Task> tempor;
               
               while(!processor[proc].empty()){
                   Task y=processor[proc].front();
                   if(y.flag==0){tempor.push(y); processor[proc].pop(); continue;}
                   if(time[proc] + x.process_time <= x.deadline){tempor.push(y); processor[proc].pop(); continue;}
                   if(y.start_time!=-1){
                      cout<<"Task "<<y.tid<<" Dropped by hard-soft\n";
                      time[proc]=-(x.end_time-t-1);
                      dropsoft++;
                   }
                    dropsoft++;
                    time[proc]-=y.process_time;
                    cout<<"Task "<<y.tid<<" Dropped By hard-soft\n";
                    processor[proc].pop();
               }
               
               if(time[proc] + x.process_time <= x.deadline){
                 tempor.push(x);
                 while(!tempor.empty()){
                  processor[proc].push(tempor.front());
                  tempor.pop();
                 }
                 cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<"\n";
               }
               temp++;
            }
            
            if(temp==2*m){not_sch.push_back(x);}
          }
          else{
            int proc=rand_proc(m);
            
            if(max(t+1,time[proc]) + x.process_time <= x.deadline){
                 processor[proc].push(x);
                 if(time[proc]==0 || time[proc]<=t){time[proc]=t+1+x.process_time;}
                 else{time[proc]+=x.process_time;}
                 cout<<"Task "<<x.tid<<" assigned to processor "<<proc<<"\n";
            }
            else{
               not_sch.push_back(x);
            
            }
          }
       }
       intask=not_sch;
    
    }
    t++;
 
 
 
 }
 
 cout<<"Number of hard dropped - "<<drophard<<"\n";
 cout<<"Number of soft dropped - "<<dropsoft<<"\n";
 


      
   
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
