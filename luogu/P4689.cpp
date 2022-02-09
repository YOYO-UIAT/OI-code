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
using pii=pair<int,int>;
const int N=1e5+5,LogN=20,M=5e5+5;
int n,m,a[N],col[N],dfn[N],out[N],dfx,anc[N][LogN],dep[N];
vector<int> e[N];
void Dfs(int u){
	dfn[u]=++dfx,dep[u]=dep[anc[u][0]]+1;
	For(j,1,LogN-1) anc[u][j]=anc[anc[u][j-1]][j-1];
	for(int v:e[u]) if(v!=anc[u][0]) anc[v][0]=u,Dfs(v);
	out[u]=dfx;
}
int Get(int u,int fa){
	if(dfn[u]<dfn[fa]||dfn[u]>out[fa]) return fa;
	Dec(j,LogN-1,0){
		int f=anc[u][j];
		if(f&&dep[f]>dep[fa]) u=f;
	}
	return u;
}
int bel[N];
struct Query{
	int l,r,i,w;
	bool operator<(const Query &x){
		if(bel[l]!=bel[x.l]) return l<x.l;
		return r==x.r?0:((r<x.r)^(bel[l]&1));
	}
}qry[M*10];
int lenq;
void AddQuery(int i,int l,int r,int l_,int r_){
	if(l>r||l_>r_) return;
	qry[++lenq]={r,r_,i,1};
	if(l>1) qry[++lenq]={l-1,r_,i,-1};
	if(l_>1) qry[++lenq]={l_-1,r,i,-1};
	if(l>1&&l_>1) qry[++lenq]={l-1,l_-1,i,1};
}
ll ans[M],cnt1[N],cnt2[N],cur;
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	vector<int> disc;
	For(i,1,n) cin>>a[i],disc.push_back(a[i]);
	sort(disc.begin(),disc.end());
	For(i,1,n-1){
		int u,v;cin>>u>>v;
		e[u].push_back(v),e[v].push_back(u);
	}
	int rt=1,cnt=0;
	Dfs(rt);
	For(i,1,n) col[dfn[i]]=lower_bound(disc.begin(),disc.end(),a[i])-disc.begin()+1;
	For(_,1,m){
		int op,x,y;cin>>op>>x;
		if(op==1) rt=x;
		else{
			cin>>y;
			int u=Get(rt,x),v=Get(rt,y);
			vector<pii> q1,q2;
			if(x==rt) q1.push_back({1,n});
			else if(u==x) q1.push_back({dfn[u],out[u]});
			else q1.push_back({1,dfn[u]-1}),q1.push_back({out[u]+1,n});
			if(y==rt) q2.push_back({1,n});
			else if(v==y) q2.push_back({dfn[v],out[v]});
			else q2.push_back({1,dfn[v]-1}),q2.push_back({out[v]+1,n});
			++cnt;
			for(auto q:q1) for(auto qq:q2) AddQuery(cnt,q.first,q.second,qq.first,qq.second);
		}
	}
	int bsiz=n/sqrt(m);
	For(i,1,n) bel[i]=(i-1)/bsiz+1;
	sort(qry+1,qry+lenq+1);
	for(int i=1,l=0,r=0;i<=lenq;++i){
		while(l<qry[i].l) ++l,++cnt1[col[l]],cur+=cnt2[col[l]];
		while(l>qry[i].l) --cnt1[col[l]],cur-=cnt2[col[l]],--l;
		while(r<qry[i].r) ++r,++cnt2[col[r]],cur+=cnt1[col[r]];
		while(r>qry[i].r) --cnt2[col[r]],cur-=cnt1[col[r]],--r;
		ans[qry[i].i]+=qry[i].w*cur;
	}
	For(i,1,cnt) cout<<ans[i]<<'\n';
	return 0;
}
