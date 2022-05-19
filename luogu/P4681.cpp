#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=1e5+5,V=1e4+5,Circ=200;
int Phi(int x){
	int phi=x;
	for(int i=2;i*i<=x;++i){
		if(x%i==0){
			phi=phi/i*(i-1);
			while(x%i==0) x/=i;
		}
	}
	if(x>1) phi=phi/x*(x-1);
	return phi;
}
int n,m,mod,a[N],occ[V],lim,circ;
struct SegTree{
	struct Node{
		int l,r,cnt,s,v[Circ],Sq;
	}t[N<<2];
	void Pushup(int p){
		t[p].cnt=min(t[p*2].cnt,t[p*2+1].cnt);
		t[p].s=t[p*2].s+t[p*2+1].s;
		For(i,1,circ) t[p].v[i]=t[p*2].v[i]+t[p*2+1].v[i];
	}
	void PushSq(int p,int k){
		k%=circ;
		rotate(t[p].v+1,t[p].v+k+1,t[p].v+circ+1);
		t[p].s=t[p].v[1];
		(t[p].Sq+=k)%=circ;
	}
	void Pushdown(int p){
		if(t[p].Sq) PushSq(p*2,t[p].Sq),PushSq(p*2+1,t[p].Sq),t[p].Sq=0;
	}
	void Build(int p,int l,int r){
		t[p].l=l,t[p].r=r,t[p].cnt=0;
		if(l==r){t[p].s=a[l];return;}
		int mid=(l+r)/2;
		Build(p*2,l,mid),Build(p*2+1,mid+1,r);
		Pushup(p);
	}
	void Modify(int p,int l,int r){
		if(l<=t[p].l&&t[p].r<=r&&t[p].cnt>=lim){PushSq(p,1);return;}
		if(t[p].l==t[p].r){
			t[p].s=t[p].s*t[p].s%mod;
			++t[p].cnt;
			if(t[p].cnt>=lim){
				t[p].v[1]=t[p].s;
				For(i,2,circ) t[p].v[i]=t[p].v[i-1]*t[p].v[i-1]%mod;
			}
			return;
		}
		Pushdown(p);
		int mid=(t[p].l+t[p].r)/2;
		if(l<=mid) Modify(p*2,l,r);
		if(r>mid) Modify(p*2+1,l,r);
		Pushup(p);
	}
	int Query(int p,int l,int r){
		if(l>t[p].r||r<t[p].l) return 0;
		if(l<=t[p].l&&t[p].r<=r) return t[p].s;
		Pushdown(p);
		return Query(p*2,l,r)+Query(p*2+1,l,r);
	}
}seg;
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m>>mod;
	For(i,1,n) cin>>a[i];
	int phi=Phi(mod);
	for(int i=0,x=1;i<=mod;++i,x=x*2%phi){
		if(occ[x]){lim=occ[x],circ=i-occ[x];break;}
		occ[x]=i;
	}
	// Debug("lim:%d,circ:%d\n",lim,circ);
	seg.Build(1,1,n);
	For(_,1,m){
		int op,x,y;cin>>op>>x>>y;
		if(op==1){
			seg.Modify(1,x,y);
		}else{
			cout<<seg.Query(1,x,y)<<'\n';
		}
	}
	return 0;
}
