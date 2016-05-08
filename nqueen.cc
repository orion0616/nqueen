#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_set>
#include <sys/time.h>

using namespace std;

class nqueen{
public:
        nqueen(int n){
                size = n;
                board = new int*[n];
                for(int i=0;i<n;i++){
                        board[i] = new int[n];
                }
                for(int i=0;i<n;i++){
                        for(int j=0;j<n;j++){
                                board[i][j] = 0;
                        }
                }
        }
        nqueen(nqueen& beforestate){
                size = beforestate.size;
                board = new int*[size];
                for(int i=0;i<size;i++){
                        board[i] = new int[size];
                }
                for(int i=0;i<size;i++){
                        for(int j=0;j<size;j++){
                                board[i][j] = 0;
                        }
                }
                for(int i=0;i<size;i++){
                        for(int j=0;j<size;j++){
                                board[i][j] = beforestate.board[i][j];
                        }
                }
        }
        ~nqueen(){
                for(int i=0;i<size;i++){
                        delete[] board[i];
                }
                delete board;
        }
        int size;
        int** board; //0は未割り当て,1はqueen,2は置かない
        bool isComplete(){
                int count = 0;
                for(int i=0;i<size;i++){
                        for(int j=0;j<size;j++){
                                if(board[i][j]==1)
                                        count++;
                        }
                }
                return count == size;
        }
        bool checkConstraint(int var){
                return checkVertical(var) && checkHorizontal(var) && checkSlanting(var);
        }
        bool checkVertical(int var){
                // int row = var/size;
                int column = var%size;
                int countnothing = 0;
                int countqueen = 0;
                for(int i=0;i<size;i++){
                        if(board[i][column]==1)
                                countqueen++;
                        if(board[i][column]==2)
                                countnothing++;
                }
                if(countqueen >= 2 || countnothing == size)
                        return false;
                return true;
        }
        bool checkHorizontal(int var){
                int row = var/size;
                // int column = var%size;
                int countnothing = 0;
                int countqueen = 0;
                for(int i=0;i<size;i++){
                        if(board[row][i]==1)
                                countqueen++;
                        if(board[row][i]==2)
                                countnothing++;
                }
                if(countqueen >= 2 || countnothing == size)
                        return false;
                return true;
        }
        bool checkSlanting(int var){
                int row = var/size;
                int column = var%size;
                int diff = column-row;
                int countnothing = 0;
                int countqueen = 0;
                for(int i=0;i<size;i++){
                        if(i+diff<=0 || i+diff >=size)
                                continue;
                        if(board[i][i+diff]==1)
                                countqueen++;
                        if(board[i][i+diff]==2)
                                   countnothing++;
                }
                if(countqueen >= 2 || countnothing == size)
                        return false;
                return true;
        }
        void printBoard(){
                for(int i=0;i<size;i++){
                        for(int j=0;j<size;j++){
                                if(board[i][j]==1)
                                        cout << "*";
                                else
                                        cout << "-";
                        }
                        cout << endl;
                }
                cout << endl;
        }
};

class choicelist{
public:
        unordered_set<int> falselist;
        unordered_set<int> unassigned;
};

void printTime(timeval t0, timeval t1){
        t1.tv_sec -= t0.tv_sec;
        if (t1.tv_usec < t0.tv_usec) {
                t1.tv_sec -= 1;
                t1.tv_usec += 1000000 - t0.tv_usec;
        } else {
                t1.tv_usec -= t0.tv_usec;
        }
        printf("%ld.%06d sec\n\n", t1.tv_sec, t1.tv_usec);
}

void updateList(nqueen& state, choicelist& choices, int whrere, int what){
        choices.unassigned.erase(whrere);
        choices.falselist.erase(whrere);
        if(what==2)
                return;
        int s = state.size;
        int row = whrere/s;
        int column = whrere%s;
        int diff = column-row;
        for(int i=0;i<s;i++){
                if(row == i)
                        continue;
                if(choices.unassigned.find(i*state.size+column)!=choices.unassigned.end()){
                        choices.unassigned.erase(i*state.size+column);
                        choices.falselist.insert(i*state.size+column);
                }
        }
        for(int i=0;i<s;i++){
                if(column == i)
                        continue;
                if(choices.unassigned.find(row*state.size+i)!=choices.unassigned.end()){
                        choices.unassigned.erase(row*state.size+i);
                        choices.falselist.insert(row*state.size+i);
                }
        }
        for(int i=0;i<s;i++){
                if(row == i)
                        continue;
                if(i+diff<=0 || i+diff >=s)
                        continue;
                if(choices.unassigned.find(i*state.size+diff)!=choices.unassigned.end()){
                        choices.unassigned.erase(i*state.size+i+diff);
                        choices.falselist.insert(i*state.size+i+diff);
                }
        }
}

bool btandfcandmrv(nqueen state,choicelist choices,int chosen = -1,int what = 2){
        if(state.isComplete()){
                // state.printBoard();
                return true;
        }
        if(chosen != -1){
                updateList(state,choices,chosen,what);
        }
        //falselistを消してく
        if(!choices.falselist.empty()){
                auto it = choices.falselist.begin();
                int var = *it;
                state.board[var/state.size][var%state.size] = 2;
                if(state.checkConstraint(var)){
                        if(btandfcandmrv(state,choices,var,2))
                                return true;
                        else
                                state.board[var/state.size][var%state.size] = 0;
                }
                else
                        state.board[var/state.size][var%state.size] = 0;
        }
        //falselist空なので，何か決めていれてく
        else{
                auto it = choices.unassigned.begin();
                int var = *it;
                for(int i=1;i<=2;i++){
                        state.board[var/state.size][var%state.size] = i;
                        if(state.checkConstraint(var)){
                                if(btandfcandmrv(state,choices,var,i))
                                        return true;
                                else{
                                        state.board[var/state.size][var%state.size] = 0;
                                }
                        }
                        else
                                state.board[var/state.size][var%state.size] = 0;
                }
        }
        return false;
}

int main(){
        int n;
        cin >> n;
        nqueen init(n);
        int nn = n*n;
        choicelist choices;
        for(int i=0;i<nn;i++){
                choices.unassigned.insert(i);
        }
        struct timeval start,goal;
        gettimeofday(&start,NULL);
        btandfcandmrv(init,choices);
        gettimeofday(&goal,NULL);
        printTime(start,goal);
}