#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#pragma GCC target("avx,sse2,sse3,sse4,mmx")
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=1e5+5,V=5e5+5;
int n,m,a[N];
int *New(int siz){
	static int pool[N*400],*cur=pool;
	cur+=siz;
	return cur-siz;
}
struct BIT{
	ll sum[N];
	void Add(int p,ll k){for(;p<=n;p+=p&-p) sum[p]+=k;}
	ll Query(int p){ll res=0;for(;p;p-=p&-p) res+=sum[p];return res;}
}bit;
struct DSU{
	int siz,*fa;
	DSU(){}
	DSU(int _siz):siz(_siz),fa(New(siz+1)){For(i,1,siz) fa[i]=i;}
	int Find(int u){return fa[u]==u?u:(fa[u]=Find(fa[u]));}
}dsu[V];
int *p[V],cnt[V];
void Div(int l,int r,int x){
	int L=lower_bound(p[x]+1,p[x]+cnt[x]+1,l)-p[x];
	int R=upper_bound(p[x]+1,p[x]+cnt[x]+1,r)-p[x]-1;
	if(L>cnt[x]) return;
	for(int u=dsu[x].Find(L);u<=R;u=dsu[x].Find(u+1)){
		if(a[p[x][u]]%x==0){
			a[p[x][u]]/=x;
			bit.Add(p[x][u],1LL*a[p[x][u]]*(1-x));
		}
		if(u==cnt[x]) break;
		if(a[p[x][u]]%x) dsu[x].fa[u]=dsu[x].Find(u+1);
	}
}
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	int mx=0;
	For(i,1,n) cin>>a[i],++cnt[a[i]],mx=max(mx,a[i]),bit.Add(i,a[i]);
	For(i,1,mx){
		For(j,2,mx/i){
			cnt[i]+=cnt[i*j];
		}
	}
	For(i,1,mx){
		if(cnt[i]){
			p[i]=New(cnt[i]+1);
			dsu[i]=DSU(cnt[i]);
			cnt[i]=0;
		}
	}
	For(i,1,n){
		for(int x=1;x*x<=a[i];++x){
			if(a[i]%x==0){
				p[x][++cnt[x]]=i;
				if(x*x!=a[i]) p[a[i]/x][++cnt[a[i]/x]]=i;
			}
		}
	}
	ll last=0;
	For(_,1,m){
		int op;ll l,r;cin>>op>>l>>r;
		l^=last,r^=last;
		if(op==1){
			ll x;cin>>x;x^=last;
			if(x>1) Div(l,r,x);
		}else{
			cout<<(last=bit.Query(r)-bit.Query(l-1))<<'\n';
		}
	}
	return 0;
}
