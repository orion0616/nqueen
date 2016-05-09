#ifndef UNASSIGNED_BINARYHEAP_H_
#define UNASSIGNED_BINARYHEAP_H_

#include "BinaryHeap.h"
#include <unordered_set>
#include <utility>

using namespace std;

class UnassignedBinaryHeap : public BinaryHeap<candidate>{
public:
        bool add(candidate x);
        void bubbleUp(int i);
        void trickleDown(int i);
        candidate remove();
        int find(int i);
};

bool UnassignedBinaryHeap::add(candidate x){
        a.push_back(x);
        n++;
        index[x.first] = n-1;
        bubbleUp(n-1);
        return true;
}
void UnassignedBinaryHeap::bubbleUp(int i){
        int p = parent(i);
        while(i>0 && (a[i] < a[p])){
                candidate tmp = a[i];
                a[i] = a[p];
                a[p] = tmp;
                index[a[i].first] = i;
                index[a[p].first] = p;
                i = p;
                p = parent(i);
        }
}
void UnassignedBinaryHeap::trickleDown(int i){
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
                        candidate tmp = a[i];
                        a[i] = a[j];
                        a[j] = tmp;
                        index[a[i].first] = i;
                        index[a[j].first] = j;
                }
                i = j;
        }while(i>=0);
}
candidate UnassignedBinaryHeap::remove(){
        candidate x = a[0];
        a[0] = a[--n];
        a.pop_back();
        index[a[0].first] = 0;
        index.erase(x.first);
        trickleDown(0);
        return  x;
}
int UnassignedBinaryHeap::find(int i){
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