#include <bits/stdc++.h>

using namespace std;

int randI(int low,
          int upp) {  // function to generate random Intergers b/w l to r

    int x = rand();

    x = x % (upp - low + 1);
    if (x < 0) {
        x = x + upp - low + 1;
    }
    x = x + low;
    return x;
}

double genrand(double low,
               double high) {  // function to generate real number b/w l to r
    int x = rand();

    double ans = low + (double)(double)x / (double)(RAND_MAX / (high - low));
    return ans;
}


vector<pair<int, int>> serverlist;
vector<pair<int, int>> userlist;
vector<int> ar_time;
vector<float> exec_time;
vector<int> flag;


int serverIndex = 0;
int userIndex = 0;
int execIndex = 0;

pair<int, int> getServer(){
    auto ret = serverlist[serverIndex];
    serverIndex++;
    return ret;
}
pair<int, int> getUser(){
    auto ret = userlist[userIndex];
    userIndex++;
    return ret;
}

int getar_time(){
    auto ret = ar_time[execIndex];
    return ret;
}

float getexec_time(){
    auto ret = exec_time[execIndex];
    return ret;
}

int getflag(){
    auto ret = flag[execIndex];
    execIndex++;
    return ret;
}



void loadServers(string filename){
    serverIndex = 0;
    serverlist.clear();
    ifstream fin;
    fin.open(filename, ios::in);
    srand(time(NULL));
    int m;
    fin>>m;
    int ct=1;
    while(ct<=m){
        int x,y;
        fin>>x>>y;
        serverlist.push_back({x,y});
        ct++;
    }
    fin.close();
}
void loadUsers(string filename){
    userIndex = 0;
    userlist.clear();
    ifstream fin;
    fin.open(filename, ios::in);
    srand(time(NULL));
    int m;
    fin>>m;
    int ct=1;
    while(ct<=m){
        int x,y;
        fin>>x>>y;
        userlist.push_back({x,y});
        ct++;
    }
    fin.close();
}

void loadExec(string filename){
    execIndex=0;
    ar_time.clear();
    exec_time.clear();
    flag.clear();
    ifstream fin;
    fin.open(filename, ios::in);
    srand(time(NULL));
    int m;
    fin>>m;
    while(m--){
       float art,flt;
       float exe;
       fin>>art>>exe>>flt;
       int ar = art;
       int fl = flt;
       ar_time.push_back(ar);
       exec_time.push_back(exe);
       flag.push_back(fl);
    }
    fin.close();
}


int main(int argc, char* argv[]) {


   if(argc<3){
    cout<<"Please provide correct arguments\n";
    return 0;
   }
   string serverfile=argv[1];
   string userfile=argv[2];

    vector<int> task_count = {100,200,500,1000};  // total different tasks
    vector<int> processor = {50,100,500};  // total different processors

    int mx = 2000;  // max time
    int batch = 3;  // batch size

    srand(10);  // to generate different number at each rand() call

    int grid = 200;  // grid size

    for (auto tskc : task_count) {
        for (auto proc : processor) {

            loadServers(serverfile);
            loadUsers(userfile);
            loadExec((to_string(tskc) + ".txt"));
            ofstream fout;  // file to write generated data

            string filename = to_string(tskc) + "X" + to_string(proc) + ".txt";
            fout.open(filename, ios::out);
            fout << proc << "\n";

            set<pair<int, int>> s;  // store the base stations and vehicular
                                    // unit's as they should be different
            int temp = proc;
            while (temp--) {  // print random locations of bs

                auto pp2 = getServer();
                // int x = randI(1, grid);
                // int y = randI(1, grid);
                int x1 = pp2.first;
                int y1 = pp2.second;


                if (s.find({x1, y1}) != s.end()) {
                    temp++;
                    continue;
                }
                s.insert({x1, y1});

                fout << x1 << " " << y1 << "\n";
            }

            vector<vector<int>> task(
                tskc, vector<int>(6));  //  store specifics for each task

            for (int i = 0; i < tskc; i++) {
                int arrival = getar_time();

                int ar_batch = (arrival / batch) * batch + batch;

                int deadline = randI(ar_batch, mx);

                int process_time = getexec_time() * (deadline - ar_batch - 1) + 1;

                int fl = getflag();

                auto pp2 = getUser();
                // int x = randI(1, grid);
                // int y = randI(1, grid);
                int x = pp2.first;
                int y = pp2.second;

                if (process_time == 0 || deadline - ar_batch <= 1 ||
                    s.find({x, y}) != s.end()) {
                    i--;
                    execIndex--;
                    continue;
                }
                // store all the things
                task[i][0] = arrival;
                task[i][1] = process_time;
                task[i][2] = deadline;
                task[i][3] = fl;
                task[i][4] = x;
                task[i][5] = y;
            }
            // sort based on arrival time
            sort(task.begin(), task.end());

            int tid = 1;
            // write to file
            for (auto x : task) {
                fout << x[0] << " " << x[1] << " " << x[2] << " " << x[3] << " "
                     << x[4] << " " << x[5] << " " << tid << "\n";
                tid++;
            }
        }
    }

    return 0;
}
