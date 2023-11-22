//
// Created by fivv on 2023/11/10.
//

#include "iostream"

using namespace std;
const int N = 1000;
int block[N], ask[N], vis[N];
int blockNum, askNum;
int idx = 0;

void init() {
    cout << "Please input the number of physical blocks: " << endl;
    cin >> blockNum;
    cout << "Please input the number of pages: " << endl;
    cin >> askNum;
    for (int i = 0; i < askNum; i++) {
        ask[i] = (rand() % (9 - 1 + 1)) + 1; // generate random number in [1, 9]
        cout << ask[i] << ' ';
    }
    cout << endl;
    cout << "  Current page  |";
    for (int i = 0; i < blockNum; i++) {
        cout << "  block" << i << "  visit bit  |";
    }
    cout << "  if page fault  |";
    cout << "  out page  |";
    cout << "  page of pointer  " << endl;
}

void show(int i, int out, bool flag) {
    cout << "       " << ask[i] << "        |";
    for (int j = 0; j < blockNum; j++) {
        if (block[j] == 0) cout << "  empty              |";
        else cout << "    " << block[j] << "         " << vis[j] << "      |";
    }
    if (flag) cout << "  no page fault  |";
    else cout << "    page fault   |";
    if (out == -1) cout << "     --     |";
    else cout << "      " << ask[out] << "     |";

    cout << "    " << block[idx % blockNum];
    cout << "\n----------------------------------------------------------------------------------------------------------------------------------"
         << endl;
}

int main() {
    init();
    for (int i = 0; i < askNum; i++) {
        int curPage = ask[i];

        bool havCurPage = false;
        for (int j = 0; j < blockNum; j++) {
            if (block[j] == curPage) {
                havCurPage = true;
                vis[j] = 1;
                break;
            }
        }
        if (havCurPage) {
            show(i, -1, true);
            continue;
        }

        bool havEmptyBlock = false;
        for (int j = 0; j < blockNum; j++) {
            if (block[j] == 0) {
                block[j] = curPage;
                vis[j] = 1;
                havEmptyBlock = true;
                break;
            }
        }
        if (havEmptyBlock) {
            show(i, -1, false);
            continue;
        }

        int outIdx = -1;
        while (true) {
            if (vis[idx] == 1) {
                vis[idx] = 0;
                idx = (idx + 1) % blockNum;
            } else {
                outIdx = idx;
                block[idx] = curPage;
                vis[idx] = 1;
                idx = (idx + 1) % blockNum;
                break;
            }
        }
        show(i, outIdx, false);

    }
    return 0;
}