#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
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

bool updateListbyFC(const nqueen& state, UnassignedBinaryHeap& choices, int where){
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

typedef pair<int,int> Arc;

bool issatisfy(Arc arc,int x,int y){
        int diff = arc.second-arc.first;
        if(y == x)
                return false;
        if(y == x+diff)
                return false;
        if(y == x-diff)
                return false;
        return true;
}

int remove_inconsistent_values(nqueen& state, UnassignedBinaryHeap& choices, Arc arc){
        //return
        //1->removed
        //0->no removed
        //-1->no arc consistent
        int removed =0;
        bool flag,flagx;
        vector<int> deletelist;
        int wherex = choices.find(arc.first);
        int wherey = choices.find(arc.second);
        unordered_set<int> x,y;
        //assignedです
        if(wherex == -1){
                x.insert(state.board[arc.first]);
                flagx = true;
        }else{
                x = choices.a[wherex].second;
                flagx = false;
        }
        if(wherey == -1){
                y.insert(state.board[arc.second]);
        }else{
                y = choices.a[wherey].second;
        }
        for(auto it = x.begin();it!=x.end();it++){
                flag = true;
                for(auto it2 = y.begin();it2!=y.end();it2++){
                        if(issatisfy(arc,*it,*it2)){
                                flag = false;
                                break;
                        }
                }
                if(flag){
                        deletelist.push_back(*it);
                        removed = 1;
                }
        }
        if(flagx){
                if(deletelist.size()==1)
                        removed = -1;
        }else{
                for(int i=0;i<deletelist.size();i++){
                        choices.a[wherex].second.erase(deletelist[i]);
                }
        }
        return removed;
}

bool updateListbyAC3(nqueen& state, UnassignedBinaryHeap& choices){
        queue<Arc> Q;
        Arc arc;
        for(int i=0;i<state.size;i++){
                for(int j=0;j<state.size;j++){
                        if(i!=j)
                                Q.push(make_pair(i,j));
                }
        }
        while(!Q.empty()){
                arc = Q.front();
                Q.pop();
                int result = remove_inconsistent_values(state,choices,arc);
                if(result == 1){
                        int dst = arc.first;
                        for(int src=0;src<state.size;src++){
                                if(src!=arc.first)
                                        Q.push(make_pair(src,dst));
                        }
                }
                else if(result == -1){
                        return false;
                }
        }
        return true;
}

bool btandfcandmrv(nqueen state,UnassignedBinaryHeap choices, int chosen = -1){
        if(state.isComplete()){
                state.printBoard();
                return true;
        }
        if(chosen != -1){
                if(!updateListbyAC3(state,choices)){
                        return false;
                }
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
