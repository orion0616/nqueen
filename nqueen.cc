#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_set>
#include <sys/time.h>
#include "printTime.h"
#include "BinaryHeap.h"
#include "unassigned_binary_heap.h"

using namespace std;

class nqueen{
public:
        nqueen(int n){
                size = n;
                board = new int[n];
                for(int i=0;i<n;i++){
                        board[i] = -1;
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
        bool isComplete(){
                for(int i=0;i<size;i++){
                        if(board[i]==-1)
                                return false;
                }
                return true;
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

bool updateList(const nqueen& state, UnassignedBinaryHeap& choices, int where){
        int num;
        for(int i=0;i<state.size;i++){
                int diff = i-where;
                if(where == i)
                        continue;
                if((num = choices.find(i)) != -1){
                        //たて
                        choices.a[num].second.erase(state.board[where]);
                        //斜め
                        if(state.board[where]+diff < state.size){
                                choices.a[num].second.erase(state.board[where]+diff);
                        }
                        if(state.board[where]-diff >= 0){
                                choices.a[num].second.erase(state.board[where]-diff);
                        }
                        if(choices.a[num].second.empty()){
                                return false;
                        }
                        choices.bubbleUp(num);
                }
        }
        return true;
}

bool btandfcandmrv(nqueen state,UnassignedBinaryHeap choices,int chosen = -1){
        if(state.isComplete()){
                state.printBoard();
                return true;
        }
        if(chosen != -1){
                if(!updateList(state,choices,chosen))
                        return false;
        }
        int var = -1;
        candidate x;
        while(var == -1){
                x = choices.findMin();
                if(state.board[x.first] == -1){
                        var = x.first;
                }
        }
        choices.remove();
        for(auto it = x.second.begin();it!=x.second.end();it++){
                int value = *it;
                state.board[var] = value;
                if(state.checkConstraint(var)){
                        bool result = btandfcandmrv(state,choices,var);
                        if(result)
                                return true;
                        else{
                                state.board[var] = -1;
                        }
                }
                else{
                        state.board[var] = -1;
                }
        }
        return false;
}

int main(){
        int n;
        cin >> n;
        nqueen init(n);
        unordered_set<int> all;
        UnassignedBinaryHeap choices;
        for(int i=0;i<n;i++){
                all.insert(i);
        }
        for(int i=0;i<n;i++){
                choices.add(make_pair(i,all));
        }
        struct timeval start,goal;
        gettimeofday(&start,NULL);
        btandfcandmrv(init,choices);
        gettimeofday(&goal,NULL);
        printTime(start,goal);
}
