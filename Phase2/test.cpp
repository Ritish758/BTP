#include<bits/stdc++.h>

using namespace std;

int randI(int low, int upp){ // function to generate random Intergers b/w l to r

    int x= rand();
    
    x=x%(upp-low+1); 
    if(x<0){x=x+upp-low+1;}
    x=x+low;
    return x;
}

double genrand(double low,double high){  // function to generate real number b/w l to r
    int x=rand();

    double ans = low + (double)(double)x/(double)(RAND_MAX/(high-low));
    return ans;
}

int main(){

    vector<int> task_count = {10};  // total different tasks
    vector<int> processor = {5}; // total different processors

    int mx=2000;  // max time 
    int batch=3;  // batch size

    srand(10);  // to generate different number at each rand() call
    
    int grid = 200;  // grid size

    for(auto tskc : task_count){
        for(auto proc : processor){

            ofstream fout;  // file to write generated data

            string filename =to_string(tskc) + "X" + to_string(proc) + ".txt";
            fout.open(filename,ios::out);
            fout << proc<<"\n";
            
            int temp = proc;
            while(temp--){  // print random locations of bs
              int x1 = randI(1,grid);
              int y1 = randI(1,grid);
              
              fout<<x1<<" "<<y1<<"\n";
            }

            vector<vector<int>> task(tskc, vector<int>(6));  //  store specifics for each task

            for(int i=0; i<tskc; i++){
                int arrival = randI(0,mx-2*batch);

                int ar_batch = (arrival/batch)*batch + batch;

                int deadline = randI(ar_batch, mx);

                int process_time = genrand(0,1) * (deadline - ar_batch);

                int flag = randI(0,1);
                
                int x = randI(1,grid);
                int y = randI(1,grid);
                
                if(process_time==0 || deadline-ar_batch <=1){i--; continue;}
                // store all the things
                task[i][0]=arrival;
                task[i][1]=process_time;
                task[i][2]=deadline;
                task[i][3]=flag;
                task[i][4]=x;
                task[i][5]=y;
            }
            // sort based on arrival time
            sort(task.begin(),task.end());
            
            int tid=1;
             // write to file
            for(auto x: task){
                fout<<x[0] << " "<<x[1] <<" "<< x[2]<<" "<<x[3]<<" "<<x[4]<<" "<<x[5]<<" "<<tid<<"\n";
                tid++;
            }

        }
    }

    return 0;

}
