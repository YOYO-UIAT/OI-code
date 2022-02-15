#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cmath>
#include <tuple>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=2e5+5,Inf=0x3f3f3f3f;
int Abs(int x){return x<0?-x:x;}
int n,m,a[N],bsiz,lb[N],rb[N],bel[N];
struct Query{
	int l,r,i;
	bool operator<(const Query &x) const{
		return bel[l]!=bel[x.l]?l<x.l:r<x.r;
	}
}q[N];
int cur,mn[N],mx[N];
vector<tuple<int,int,int,int>> oper;
void Add(int p){
	oper.emplace_back(p,mn[a[p]],mx[a[p]],cur);
	mn[a[p]]=min(mn[a[p]],p);
	mx[a[p]]=max(mx[a[p]],p);
	cur=max(cur,mx[a[p]]-mn[a[p]]);
}
void RollBack(unsigned ver){
	while(oper.size()>ver){
		auto x=oper.back();oper.pop_back();
		mn[a[get<0>(x)]]=get<1>(x),mx[a[get<0>(x)]]=get<2>(x);
		cur=get<3>(x);
	}
}
int ans[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n;
	vector<int> disc;
	For(i,1,n) cin>>a[i],disc.push_back(a[i]);
	sort(disc.begin(),disc.end());
	For(i,1,n) a[i]=lower_bound(disc.begin(),disc.end(),a[i])-disc.begin()+1;
	cin>>m;
	For(i,1,m) cin>>q[i].l>>q[i].r,q[i].i=i;
	bsiz=max(1,int(n/sqrt(m)));
	For(i,1,n) bel[i]=(i-1)/bsiz+1,rb[bel[i]]=i;
	Dec(i,n,1) lb[bel[i]]=i;
	sort(q+1,q+m+1);
	For(i,1,n) mn[i]=Inf;
	for(int i=1,l=1,r=0,pre=-1;i<=m;++i){
		if(bel[q[i].l]!=pre){
			RollBack(0);l=rb[bel[q[i].l]],r=l-1,pre=bel[q[i].l];
		}
		if(bel[q[i].l]==bel[q[i].r]){
			For(j,q[i].l,q[i].r) Add(j);
			ans[q[i].i]=cur;RollBack(0);
			continue;
		}
		while(r<q[i].r) Add(++r);
		int ver=oper.size();
		while(l>q[i].l) Add(--l);
		ans[q[i].i]=cur;
		RollBack(ver),l=rb[bel[q[i].l]];
	}
	For(i,1,m) cout<<ans[i]<<'\n';
	return 0;
}
