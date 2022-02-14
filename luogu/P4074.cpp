#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cmath>
#include <map>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
using pii=pair<int,int>;
const int N=1e5+5,LogN=20;
int n,m,q,v[N],w[N],c[N],st[N],ed[N],orig[N*2],dfx,anc[N][LogN],dep[N];
vector<int> e[N];
void Dfs(int u){
	orig[st[u]=++dfx]=u,dep[u]=dep[anc[u][0]]+1;
	For(j,1,18) anc[u][j]=anc[anc[u][j-1]][j-1];
	for(int v:e[u]) if(v!=anc[u][0]) anc[v][0]=u,Dfs(v);
	orig[ed[u]=++dfx]=u;
}
int LCA(int u,int v){
	if(dep[u]<dep[v]) swap(u,v);
	for(int i=18,k=dep[u]-dep[v];i>=0;--i){
		if((k>>i)&1) u=anc[u][i];
	}
	if(u==v) return u;
	Dec(i,18,0) if(anc[u][i]!=anc[v][i]) u=anc[u][i],v=anc[v][i];
	return anc[u][0];
}
struct MoQuery{
	static int siz;
	int l,r,t,i,lca;
	bool operator<(const MoQuery &x) const{
		return make_tuple(l/siz,r/siz,t)<make_tuple(x.l/siz,x.r/siz,t);
	}
}qq[N];
int vis[N],cnt[N],pre[N],MoQuery::siz;
ll cur;
pii mdf[N];
void Add(int p){
	++cnt[c[p]],cur+=1LL*v[c[p]]*w[cnt[c[p]]];
}
void Del(int p){
	cur-=1LL*v[c[p]]*w[cnt[c[p]]],--cnt[c[p]];
}
void Modify(int p){
	if(vis[p]) Del(p);
	else Add(p);
	vis[p]^=1;
}
ll ans[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m>>q;
	For(i,1,m) cin>>v[i];
	For(i,1,n) cin>>w[i];
	For(i,1,n-1){
		int u,v;cin>>u>>v;
		e[u].push_back(v),e[v].push_back(u);
	}
	For(i,1,n) cin>>c[i];
	Dfs(1);
	int tim=0,lenq=0;
	map<int,int> mp;
	For(i,1,q){
		int tp,x,y;cin>>tp>>x>>y;
		if(!tp){
			mdf[++tim]={x,y};
			if(!mp.count(x)) pre[tim]=c[x];
			else pre[tim]=mp[x];
			mp[x]=y;
		}else{
			if(dep[x]>dep[y]) swap(x,y);
			int lca=LCA(x,y);++lenq;
			if(lca==x) qq[lenq]={st[x],st[y],tim,lenq,0};
			else qq[lenq]={ed[x],st[y],tim,lenq,lca};
		}
	}
	if(!q) return 0;
	if(tim) MoQuery::siz=max(1,int(cbrt(double(n)*n/q*tim)));
	else MoQuery::siz=max(1,int(n/sqrt(q)));
	sort(qq+1,qq+lenq+1);
	for(int i=1,l=1,r=0,t=0;i<=lenq;++i){
		while(t<qq[i].t){
			++t;
			if(vis[mdf[t].first]){
				Del(mdf[t].first),c[mdf[t].first]=mdf[t].second,Add(mdf[t].first);
			}else c[mdf[t].first]=mdf[t].second;
		}
		while(t>qq[i].t){
			if(vis[mdf[t].first]){
				Del(mdf[t].first),c[mdf[t].first]=pre[t],Add(mdf[t].first);
			}else c[mdf[t].first]=pre[t];
			--t;
		}
		while(l>qq[i].l) Modify(orig[--l]);
		while(r<qq[i].r) Modify(orig[++r]);
		while(l<qq[i].l) Modify(orig[l++]);
		while(r>qq[i].r) Modify(orig[r--]);
		if(qq[i].lca) Add(qq[i].lca);
		ans[qq[i].i]=cur;
		if(qq[i].lca) Del(qq[i].lca);
	}
	For(i,1,lenq) cout<<ans[i]<<'\n';
	return 0;
}
