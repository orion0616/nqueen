#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <sys/time.h>
#include <random>
#include <cmath>
#include "printTime.h"
#include "var_binary_heap.h"

using namespace std;

class nqueen{
public:
        nqueen(int n){
                random_device rnd;
                size = n;
                board = new int[n];
                for(int i=0;i<n;i++){
                        board[i] = rnd()%n;
                }
        }
        nqueen(nqueen& before){
                size = before.size;
                board = new int[size];
                for(int i=0;i<size;i++){
                        board[i] = before.board[i];
                }
        }
        ~nqueen(){
                delete board;
        }
        int size;
        int* board;
        vector<int> conflictlist;
        bool isSolution(){
                conflictlist.clear();
                unordered_set<int> index;
                for(int i=0;i<size;i++){
                        for(int j=i+1;j<size;j++){
                                int diff = j-i;
                                if(board[i]==board[j] || board[i]+diff == board[j] || board[i]-diff == board[j]){
                                        if(index.find(i) == index.end()){
                                                conflictlist.push_back(i);
                                                index.insert(i);
                                        }
                                        if(index.find(j) == index.end()){
                                                conflictlist.push_back(j);
                                                index.insert(j);
                                        }
                                }
                        }
                }
                if(conflictlist.empty())
                        return true;
                else
                        return false;
        }
        bool checkConstraint(int var){
                for(int i=0;i<size;i++){
                        if(var==i)
                                continue;
                        if(board[i]==-1)
                                continue;
                        if(board[i]==board[var]){
                                return false;
                        }
                        int diff = i-var;
                        if(board[i] == board[var]+diff){
                                return false;
                        }
                        if(board[i] == board[var]-diff){
                                return false;
                        }
                }
                return true;
        }
        void printBoard(){
                for(int i=0;i<size;i++){
                        for(int j=0;j<size;j++){
                                if(board[i]==j)
                                        cout << "*";
                                else
                                        cout << "-";
                        }
                        cout << endl;
                }
                cout << endl;
        }
};

int conflict(nqueen& state,int var){
        int diff,num;
        // int choice[state.size];
        varBinaryHeap bh;
        for(int i=0;i<state.size;i++){
                bh.add(make_pair(i,0));
        }
        for(int i=0;i<state.size;i++){
                if(var == i)
                        continue;
                else{
                        // choice[state.board[i]]++;
                        num = bh.find(state.board[i]);
                        bh.a[num].second++;
                        bh.trickleDown(num);
                        diff = abs(var-i);
                        if(state.board[i]+diff < state.size){
                                num = bh.find(state.board[i]+diff);
                                bh.a[num].second++;
                                bh.trickleDown(num);
                                // choice[state.board[i]+diff]++;
                        }
                        if(0<= state.board[i]-diff){
                                // choice[state.board[i]-diff]++;
                                num = bh.find(state.board[i]-diff);
                                bh.a[num].second++;
                                bh.trickleDown(num);
                        }
                }
        }
        return bh.findMin().first;
}

bool minconflict(nqueen& state,int maxsteps){
        for(int i=0;i<maxsteps;i++){
                // state.printBoard();
                if(state.isSolution()){
                        // state.printBoard();
                        return true;
                }
                random_device rnd;
                //conflictを起こしてるやつをrandom抽出
                int var = state.conflictlist[rnd()%state.conflictlist.size()];
                //conflictが一番少なくなる位置へ移動
                // int min = 1000000;
                // int tmp;
                // int value;
                // for(int i=0;i<state.size;i++){
                //         tmp = conflict(state,var,i);
                //         if(min > tmp){
                //                 min = tmp;
                //                 value = i;
                //         }
                // }
                int value = conflict(state,var);
                state.board[var] = value;
        }
        return false;
}

int main(){
        int n;
        cin >> n;
        nqueen init(n);
        struct timeval start,goal;
        gettimeofday(&start,NULL);
        minconflict(init,10);
        gettimeofday(&goal,NULL);
        printTime(start,goal);
}
