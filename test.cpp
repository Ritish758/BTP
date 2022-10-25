#include<bits/stdc++.h>

using namespace std;

int randI(int low, int upp){

    int x= rand();
    
    x=x%(upp-low+1); 
    if(x<0){x=x+upp-low+1;}
    x=x+low;
    return x;
}

double genrand(){
    int x=rand();

    double ans = (double)(double)x/(double)(RAND_MAX);
    return ans;
}

int main(){

    vector<int> task_count = {10, 50, 100};
    vector<int> processor = {5, 10, 20};

    int mx=2000;
    int batch=3;

    srand(10);

    for(auto tskc : task_count){
        for(auto proc : processor){

            ofstream fout;

            string filename =to_string(tskc) + "X" + to_string(proc) + ".txt";
            fout.open(filename,ios::out);
            fout << proc<<"\n";

            vector<vector<int>> task(tskc, vector<int>(4));

            for(int i=0; i<tskc; i++){
                int arrival = randI(0,mx-2*batch);

                int ar_batch = (arrival/batch)*batch + batch;


                int deadline = randI(ar_batch, mx);

                int process_time = genrand() * (deadline - ar_batch);

                int flag = randI(0,1);

                task[i][0]=arrival;
                task[i][1]=process_time;
                task[i][2]=deadline;
                task[i][3]=flag;
                //cout<< arrival << " " << deadline <<" "<< process_time <<" "<<flag <<"\n";

                //if(arrival + process_time > deadline) {cout<<"000000\n";}

            }

            sort(task.begin(),task.end());
            
            int tid=1;

            for(auto x: task){
                fout<<x[0] << " "<<x[1] <<" "<< x[2]<<" "<<x[3]<<" "<<tid<<"\n";
                tid++;
            }

        }
    }

    return 0;

}
