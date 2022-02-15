#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cmath>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=2e5+5;
int n,m,a[N];
int mex,cnt[N];
vector<tuple<int,int,int>> oper;
void Del(int p){
	oper.emplace_back(p,cnt[a[p]],mex);
	if(!--cnt[a[p]]) mex=min(mex,a[p]);
}
void RollBack(int ver){
	while(int(oper.size())>ver){
		auto x=oper.back();oper.pop_back();
		cnt[a[get<0>(x)]]=get<1>(x),mex=get<2>(x);
	}
}
int bsiz,bel[N],lb[N],rb[N],ans[N];
struct Query{
	int l,r,i;
	bool operator<(const Query &x) const{return bel[l]==bel[x.l]?r>x.r:l<x.l;}
}q[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	For(i,1,n) cin>>a[i],++cnt[a[i]];
	while(cnt[mex]) ++mex;
	For(i,1,m) cin>>q[i].l>>q[i].r,q[i].i=i;
	bsiz=max(1.,n/sqrt(m));
	For(i,1,n) bel[i]=(i-1)/bsiz+1,rb[bel[i]]=i;
	Dec(i,n,1) lb[bel[i]]=i;
	sort(q+1,q+m+1);
	for(int i=1,l=1,r=n,preb=0,pver=0;i<=m;++i){
		int curb=bel[q[i].l];
		if(curb!=preb){
			RollBack(pver),r=n;
			while(l<lb[curb]) Del(l++);
			preb=curb,pver=oper.size();
		}
		while(r>q[i].r) Del(r--);
		int ver=oper.size();
		while(l<q[i].l) Del(l++);
		ans[q[i].i]=mex;
		RollBack(ver),l=lb[curb];
	}
	For(i,1,m) cout<<ans[i]<<'\n';
	return 0;
}
