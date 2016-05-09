#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_set>
#include <sys/time.h>
#include "printTime.h"

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

typedef vector<unordered_set<int> > choicelist;

void updateList(const nqueen& state, choicelist& choices, int where){
        for(int i=0;i<state.size;i++){
                if(where == i)
                        continue;
                choices[i].erase(state.board[where]);
                int diff = i-where;
                choices[i].erase(state.board[where]+diff);
                choices[i].erase(state.board[where]-diff);
        }
        return;
}

bool btandfcandmrv(nqueen state,choicelist choices,int chosen = -1){
        if(state.isComplete()){
                state.printBoard();
                return true;
        }
        if(chosen != -1){
                updateList(state,choices,chosen);
        }
        int var = -1;
        //selece unassigned variavle(must fix)
        for(int i=0;i<state.size;i++){
                if(state.board[i]==-1){
                        var = i;
                        break;
                }
        }
        for(auto it = choices[var].begin();it!=choices[var].end();it++){
                int value = *it;
                state.board[var] = value;
                if(state.checkConstraint(var)){
                        choicelist newone = choices;
                        newone[var].clear();
                        newone[var].insert(value);
                        bool result = btandfcandmrv(state,newone,var);
                        if(result)
                                return true;
                        else
                                state.board[var] = -1;
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
        choicelist choices;
        unordered_set<int> all;
        for(int i=0;i<n;i++){
                all.insert(i);
        }
        for(int i=0;i<n;i++){
                choices.push_back(all);
        }
        struct timeval start,goal;
        gettimeofday(&start,NULL);
        btandfcandmrv(init,choices);
        gettimeofday(&goal,NULL);
        printTime(start,goal);
}