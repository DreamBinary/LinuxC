//
// Created by fivv on 2023/11/26.
//

#include <iostream>

using namespace std;

int p, r; // p ==>> number of process, r ==>> number of resource
int maxs[10][10]; // maxs[i][j] ==>> the max number of resource j that process i need
int allocation[10][10]; // allocation[i][j] ==>> the number of resource j that process i has
int need[10][10];   // need[i][j] ==>> the number of resource j that process i need
int available[10]; // available[j] ==>> the number of resource j that is available

void input() {
    cout << "Please input the number of process: " << endl;
    cin >> p;
    cout << "Please input the number of resource: " << endl;
    cin >> r;
    cout << "Please input the maxs matrix: " << endl;
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < r; j++) {
            cin >> maxs[i][j];
        }
    }
    cout << "Please input the allocation matrix: " << endl;
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < r; j++) {
            cin >> allocation[i][j];
        }
    }
    cout << "Please input the need matrix: " << endl;
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < r; j++) {
            cin >> need[i][j];
        }
    }
    cout << "Please input the available vector: " << endl;
    for (int i = 0; i < r; i++) {
        cin >> available[i];
    }
}

bool compare(int m[], int n[]) {
    for (int i = 0; i < r; i++) {
        if (m[i] < n[i]) {
            return false;
        }
    }
    return true;
}

bool check() {
    int finish[p]; // finish[i] ==>> whether process i has finished
    int work[r];
    for (int i = 0; i < p; i++) {
        finish[i] = 0;
    }
    for (int i = 0; i < r; i++) {
        work[i] = available[i];
    }
    int cnt = 0;
    while (cnt < p) {
        bool flag = false;
        for (int i = 0; i < p; i++) {
            if (finish[i] == 0 && compare(work, need[i])) {
                cout << "P" << i << " ==>> ";
                for (int j = 0; j < r; j++) {
                    work[j] += allocation[i][j];
                }
                finish[i] = 1;
                flag = true;
                cnt++;
            }
        }
        if (!flag) {
            cout << "!!!! Deadlock !!!!" << endl;
            return false;
        }
    }
    cout << "==>> No Deadlock <<==" << endl;
    return true;
}


int main() {
    input();
    check();
    return 0;
}


//4
//3
//
//3 2 2
//6 1 3
//3 1 4
//4 2 2
//
//1 0 0
//5 1 1
//2 1 1
//0 0 2
//
//2 2 2
//1 0 2
//1 0 3
//4 2 0
//
//1 1 0
