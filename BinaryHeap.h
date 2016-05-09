#ifndef BINARYHEAP_H_
#define BINARYHEAP_H_

#include <cstring>
#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef pair<int,unordered_set<int> > candidate;
bool operator<(const candidate& a,const candidate& b){
        return a.second.size() < b.second.size();
}

template<class T>
class BinaryHeap {
public:
	int n;
	virtual void bubbleUp(int i) {
		int p = parent(i);
		while (i > 0 && (a[i] < a[p])) {
			T tmp = a[i];
			a[i] = a[p];
			a[p] = tmp;
			i = p;
			p = parent(i);
		}
	}
	virtual void trickleDown(int i) {
		do {
			int j = -1;
			int r = right(i);
			if (r < n && (a[r] < a[i])) {
				int l = left(i);
				if (a[l] < a[r]) {
					j = l;
				} else {
					j = r;
				}
			} else {
				int l = left(i);
				if (l < n && (a[l] < a[i])) {
					j = l;
				}
			}
			if (j >= 0){
				T tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
			i = j;
		} while (i >= 0);
	}
	bool empty(){
		return n==0;
	}
	static int left(int i) {
		return 2*i + 1;
	}
	static int right(int i) {
		return 2*i + 2;
	}
	static int parent(int i) {
		return (i-1)/2;
	}
	vector<T> a;
        unordered_map<int,int> index;
	BinaryHeap(){n=0;}
	~BinaryHeap(){
		;
	}
	virtual bool add(T x){
                a.push_back(x);
                n++;
                bubbleUp(n-1);
                return true;
        }
	T findMin() {
		return a[0];
	}
	virtual T remove(){
                T x = a[0];
                a[0] = a[--n];
                a.pop_back();
                trickleDown(0);
                return x;
        }
	int size() {
		return n;
	}
};


#endif /* BINARYHEAP_H_ */
