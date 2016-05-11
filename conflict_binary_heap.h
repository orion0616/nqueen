#ifndef CONFLICTBINARYHEAP_H_
#define CONFLICTBINARYHEAP_H_

#include "BinaryHeap.h"
#include <unordered_set>
#include <utility>

typedef pair<int,unordered_set<int> > conf;

using namespace std;

class conflictBinaryHeap : public BinaryHeap<conf>{
public:
        bool add(conf x);
        void bubbleUp(int i);
        void trickleDown(int i);
        conf remove();
        int find(int i);
};

bool conflictBinaryHeap::add(conf x){
        a.push_back(x);
        n++;
        index[x.first] = n-1;
        bubbleUp(n-1);
        return true;
}
void conflictBinaryHeap::bubbleUp(int i){
        int p = parent(i);
        while(i>0 && (a[i] < a[p])){
                conf tmp = a[i];
                a[i] = a[p];
                a[p] = tmp;
                index[a[i].first] = i;
                index[a[p].first] = p;
                i = p;
                p = parent(i);
        }
}
void conflictBinaryHeap::trickleDown(int i){
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
                        conf tmp = a[i];
                        a[i] = a[j];
                        a[j] = tmp;
                        index[a[i].first] = i;
                        index[a[j].first] = j;
                }
                i = j;
        }while(i>=0);
}
conf conflictBinaryHeap::remove(){
        conf x = a[0];
        a[0] = a[--n];
        a.pop_back();
        index[a[0].first] = 0;
        index.erase(x.first);
        trickleDown(0);
        return  x;
}
int conflictBinaryHeap::find(int i){
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