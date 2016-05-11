#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <sys/time.h>
#include <random>
#include <cmath>
#include "printTime.h"
#include "var_binary_heap.h"
#include "conflict_binary_heap.h"

using namespace std;

class nqueen{
public:
        nqueen(int n){
                random_device rnd;
                mt19937 mt(rnd());
                size = n;
                board = new int[n];
                for(int i=0;i<n;i++){
                        board[i] = -1;
                }
                initialize();
                // for(int i=0;i<n;i++){
                //         board[i] = mt()%n;
                // }
        }
        ~nqueen(){
                delete board;
        }
        // void firstconflict(){
        //         for(int i=0;i<size;i++){
        //                 if(conflictindex.find(i) != conflictindex.end())
        //                         continue;
        //                 for(int j=0;j<size;j++){
        //                         if(i==j)
        //                                 continue;
        //                         int diff = j-i;
        //                         if(board[i]==board[j] || board[i]+diff == board[j] || board[i]-diff == board[j]){
        //                                 if(conflictindex.find(i) == conflictindex.end()){
        //                                         conflictindex.insert(i);
        //                                 }
        //                                 if(conflictindex.find(j) == conflictindex.end()){
        //                                         conflictindex.insert(j);
        //                                 }
        //                         }
        //                 }
        //         }
        // }
        void initialize(){
                //greedy
                unordered_set<int> tmp;
                for(int i=0;i<size;i++){
                        int diff,num;
                        conflictBinaryHeap bh;
                        for(int j=0;j<size;j++){
                                bh.add(make_pair(j,tmp));
                        }
                        for(int j=0;j<i;j++){
                                num = bh.find(board[j]);
                                bh.a[num].second.insert(j);
                                bh.trickleDown(num);
                                diff = abs(i-j);
                                if(board[j]+diff < size){
                                        num = bh.find(board[j]+diff);
                                        bh.a[num].second.insert(j);
                                        bh.trickleDown(num);
                                }
                                if(0<= board[j]-diff){
                                        num = bh.find(board[j]-diff);
                                        bh.a[num].second.insert(j);
                                        bh.trickleDown(num);
                                }
                        }
                        vector<pair<int,unordered_set<int> > > mins;
                        pair<int,unordered_set<int> > min = bh.findMin();
                        bh.remove();
                        mins.push_back(min);
                        while(bh.findMin().second.size() == mins[0].second.size()){
                                mins.push_back(bh.findMin());
                                bh.remove();
                                if(bh.size()==0)
                                        break;
                        }
                        random_device rnd;
                        int x = rnd()%mins.size();
                        board[i] = mins[x].first;
                        for(auto it = mins[x].second.begin();it!=mins[x].second.end();it++){
                                conflictindex.insert(*it);
                                conflictindex.insert(i);
                        }
                }
        }
        int size;
        int* board;
        unordered_set<int> conflictindex;
        unordered_set<int> conflictcandidate;
        bool isSolution(){
                for(auto it = conflictcandidate.begin();it!=conflictcandidate.end();it++){
                        for(int i=0;i<size;i++){
                                if(*it==i)
                                        continue;
                                int diff = abs(i-*it);
                                if(board[*it]==board[i] || board[*it]+diff == board[i] || board[*it]-diff == board[i]){
                                        if(conflictindex.find(*it) == conflictindex.end()){
                                                conflictindex.insert(*it);
                                        }
                                }
                        }
                }
                conflictcandidate.clear();
                if(conflictindex.empty())
                        return true;
                return false;
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
                // cout << endl;
        }
};

int conflict(nqueen& state,int var){
        int diff,num;
        varBinaryHeap bh;
        for(int i=0;i<state.size;i++){
                bh.add(make_pair(i,0));
        }
        for(int i=0;i<state.size;i++){
                if(var == i)
                        continue;
                num = bh.find(state.board[i]);
                bh.a[num].second++;
                bh.trickleDown(num);
                diff = abs(var-i);
                if(state.board[i]+diff < state.size){
                        num = bh.find(state.board[i]+diff);
                        bh.a[num].second++;
                        bh.trickleDown(num);
                }
                if(0<= state.board[i]-diff){
                        num = bh.find(state.board[i]-diff);
                        bh.a[num].second++;
                        bh.trickleDown(num);
                }
        }
        vector<pair<int,int> > mins;
        pair<int,int> min = bh.findMin();
        bh.remove();
        mins.push_back(min);
        while(bh.findMin().second == mins[0].second){
                mins.push_back(bh.findMin());
                bh.remove();
        }
        random_device rnd;
        int x = rnd()%mins.size();
        return mins[x].first;
}

void eraseconflict(nqueen& state,int var, int value){
        state.conflictindex.erase(var);
        // state.conflictcandidate.insert(var);
        for(int i=0;i<state.size;i++){
                if(var==i)
                        continue;
                int diff = i-var;
                if(value==state.board[i] || value+diff == state.board[i] || value-diff == state.board[i]){
                        state.conflictindex.erase(i);
                        state.conflictcandidate.insert(i);
                }
        }
        return;
}

void addconflict(nqueen& state, int var,int value){
        for(int i=0;i<state.size;i++){
                if(var==i)
                        continue;
                int diff = i-var;
                if(value==state.board[i] || value+diff == state.board[i] || value-diff == state.board[i]){
                        if(state.conflictindex.find(i) == state.conflictindex.end()){
                                state.conflictcandidate.insert(i);
                                state.conflictcandidate.insert(var);
                        }
                }
        }
        return;
}

bool minconflict(nqueen& state,int maxsteps){
        for(int i=0;i<maxsteps;i++){
                // cout << i <<endl;
                // state.printBoard();
                if(state.isSolution()){
                        cout << i << endl;
                        // state.printBoard();
                        return true;
                }
                // cout << " " << state.conflictindex.size() << endl <<endl;
                random_device rnd;
                //conflictを起こしてるやつをrandom抽出
                // int var = state.conflictlist[rnd()%state.conflictlist.size()];
                int x = rnd()%state.conflictindex.size();
                auto it = state.conflictindex.begin();
                for(int i=0;i<x;i++){
                        it++;
                }
                int var = *it;
                //conflictが一番少なくなる位置へ移動
                int value = conflict(state,var);
                eraseconflict(state,var,state.board[var]);
                state.board[var] = value;
                addconflict(state,var,value);
        }
        return false;
}

int main(){
        int n;
        cin >> n;
        struct timeval start,goal,ongoing;
        gettimeofday(&start,NULL);
        nqueen init(n);
        gettimeofday(&ongoing,NULL);
        printTime(start,ongoing);
        // init.firstconflict();
        minconflict(init,100000);
        gettimeofday(&goal,NULL);
        printTime(ongoing,goal);
}
