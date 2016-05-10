#ifndef UNASSIGNED_BINARYHEAP_H_
#define UNASSIGNED_BINARYHEAP_H_

#include "BinaryHeap.h"
#include <unordered_set>
#include <utility>

typedef pair<int,int> howmanyconflict;

using namespace std;

class varBinaryHeap : public BinaryHeap<howmanyconflict>{
public:
        bool add(howmanyconflict x);
        void bubbleUp(int i);
        void trickleDown(int i);
        howmanyconflict remove();
        int find(int i);
};

bool varBinaryHeap::add(howmanyconflict x){
        a.push_back(x);
        n++;
        index[x.first] = n-1;
        bubbleUp(n-1);
        return true;
}
void varBinaryHeap::bubbleUp(int i){
        int p = parent(i);
        while(i>0 && (a[i] < a[p])){
                howmanyconflict tmp = a[i];
                a[i] = a[p];
                a[p] = tmp;
                index[a[i].first] = i;
                index[a[p].first] = p;
                i = p;
                p = parent(i);
        }
}
void varBinaryHeap::trickleDown(int i){
        do{
                int j = -1;
                int r = right(i);
                if(r < n && (a[r] < a[i])){
                        int l = left(i);
                        if(a[l] < a[r]){
                                j = l;
                        }else{
                                j = r;
                        }
                } else{
                        int l = left(i);
                        if(l < n && (a[l] < a[i])){
                                j = l;
                        }
                }
                if(j >= 0){
                        howmanyconflict tmp = a[i];
                        a[i] = a[j];
                        a[j] = tmp;
                        index[a[i].first] = i;
                        index[a[j].first] = j;
                }
                i = j;
        }while(i>=0);
}
howmanyconflict varBinaryHeap::remove(){
        howmanyconflict x = a[0];
        a[0] = a[--n];
        a.pop_back();
        index[a[0].first] = 0;
        index.erase(x.first);
        trickleDown(0);
        return  x;
}
int varBinaryHeap::find(int i){
        int num;
        auto it = index.find(i);
        if(it != index.end()){
                num = (*it).second;
                return num;
        }
        else{
                return -1;
        }
}


#endif