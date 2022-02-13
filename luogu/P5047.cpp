#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=1e5+5,SqrtN=320;
struct MoQuery{
	static int siz;
	int l,r,i;
	bool operator<(const MoQuery &x) const{
		return l/siz!=x.l/siz?l<x.l:l/siz&1?r>x.r:r<x.r;
	}
}q[N];
int n,m,a[N],MoQuery::siz;
struct Bit{
	int t[N];
	void Add(int p,int k){for(;p<=n;p+=p&-p) t[p]+=k;}
	int Query(int p){int res=0;for(;p;p-=p&-p) res+=t[p];return res;}
}bit;
struct Block{
	int siz,bsum[SqrtN],psum[N],bel[N],lb[N],rb[N];
	void Build(){
		siz=sqrt(n);
		For(i,1,n) bel[i]=(i-1)/siz+1,rb[bel[i]]=i;
		Dec(i,n,1) lb[bel[i]]=i;
	}
	void Add(int p,int k){
		For(i,bel[p],bel[n]) bsum[i]+=k;
		For(i,p,rb[bel[p]]) psum[i]+=k;
	}
	int Query(int p){return p?bsum[bel[p]-1]+psum[p]:0;}
}blk;
struct Query{int tp,l,r,i,w;};
vector<Query> qq[N];
int le[N],ge[N];ll _ans[N],ans[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	vector<int> disc;
	For(i,1,n) cin>>a[i],disc.push_back(a[i]);
	sort(disc.begin(),disc.end());
	For(i,1,n) a[i]=lower_bound(disc.begin(),disc.end(),a[i])-disc.begin()+1;
	For(i,1,n) le[i]=bit.Query(a[i]-1),ge[i]=bit.Query(n)-bit.Query(a[i]),bit.Add(a[i],1);
	For(i,1,m) cin>>q[i].l>>q[i].r,q[i].i=i;
	MoQuery::siz=n/sqrt(m);
	sort(q+1,q+m+1);
	for(int i=1,l=1,r=0;i<=m;++i){
		if(l>q[i].l) qq[r].push_back({0,q[i].l,l-1,i,1});
		while(l>q[i].l) --l,_ans[i]-=le[l];
		if(r<q[i].r) qq[l-1].push_back({1,r+1,q[i].r,i,-1});
		while(r<q[i].r) ++r,_ans[i]+=ge[r];
		if(l<q[i].l) qq[r].push_back({0,l,q[i].l-1,i,-1});
		while(l<q[i].l) _ans[i]+=le[l],++l;
		if(r>q[i].r) qq[l-1].push_back({1,q[i].r+1,r,i,1});
		while(r>q[i].r) _ans[i]-=ge[r],--r;
	}
	blk.Build();
	For(i,1,n){
		blk.Add(a[i],1);
		for(const auto &x:qq[i]){
			For(j,x.l,x.r){
				if(x.tp==0) _ans[x.i]+=x.w*blk.Query(a[j]-1);
				else _ans[x.i]+=x.w*(blk.Query(n)-blk.Query(a[j]));
			}
		}
	}
	For(i,2,m) _ans[i]+=_ans[i-1];
	For(i,1,m) ans[q[i].i]=_ans[i];
	For(i,1,m) cout<<ans[i]<<'\n';
	return 0;
}
