#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
multiset<int> cnt,sum;
int n,C;
int Best(int x,bool del){
	auto it=cnt.lower_bound(C-x);
	if(it==cnt.begin()) return -1;
	--it;
	if(del&&*it==x) return it==cnt.begin()?-1:*--it;
	else return *it;
}
void Insert(int x){
	int y=Best(x,0);
	if(y!=-1){
		int z=Best(y,1);
		if(z==-1||z<x){
			if(z!=-1&&Best(z,1)==y) sum.erase(sum.find(y+z));
			sum.insert(x+y);
		}
	}
	cnt.insert(x);
}
void Delete(int x){
	cnt.erase(cnt.find(x));
	int y=Best(x,0);
	if(y!=-1){
		int z=Best(y,1);
		if(z<x){
			sum.erase(sum.find(x+y));
			if(z!=-1&&Best(z,1)==y) sum.insert(z+y);
		}
	}
}
int Query(){
	return max((*prev(cnt.end())+*prev(prev(cnt.end())))%C,sum.size()?*prev(sum.end()):0);
}
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>C;
	int last=0;
	For(_,1,n){
		int op,x;cin>>op>>x;
		x^=last;
		if(op==1) Insert(x%C);
		else Delete(x%C);
		if(cnt.size()<2) cout<<"EE"<<'\n',last=0;
		else cout<<(last=Query())<<'\n';
	}
	return 0;
}
