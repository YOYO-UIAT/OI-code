#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=5e5+5,B=16,LogBV=10,Inf=0x3f3f3f3f,Block=32;
int n,m,a[N],bcnt,L[LogBV],R[LogBV],bel[N];
int Locate(int x){return upper_bound(L,L+bcnt,x)-L-1;}
struct SegTree{
	int lb,rb;//[lb,rb)
	struct Node{
		int l,r,cnt,mx,mn;ll s,Minus;
	}t[N*8/Block];
	void Pushup(int p){
		t[p].mn=min(t[p*2].mn,t[p*2+1].mn);
		t[p].mx=max(t[p*2].mx,t[p*2+1].mx);
		t[p].s=t[p*2].s+t[p*2+1].s;
		t[p].cnt=t[p*2].cnt+t[p*2+1].cnt;
	}
	void PushMinus(int p,ll k){
		t[p].mx-=k,t[p].mn-=k,t[p].s-=t[p].cnt*k;
		t[p].Minus+=k;
	}
	void Pushdown(int p){
		if(t[p].Minus){
			PushMinus(p*2,t[p].Minus),PushMinus(p*2+1,t[p].Minus);
			t[p].Minus=0;
		}
	}
	void Pushdown2(int p){
		if(t[p].Minus){
			For(i,t[p].l,t[p].r){
				if(a[i]>=lb&&a[i]<rb) a[i]-=t[p].Minus;
			}
			t[p].Minus=0;
		}
	}
	void Pushup2(int p){
		t[p].mn=Inf,t[p].mx=-Inf,t[p].cnt=t[p].s=0;
		For(i,t[p].l,t[p].r){
			if(a[i]>=lb&&a[i]<rb){
				t[p].mn=min(t[p].mn,a[i]);
				t[p].mx=max(t[p].mx,a[i]);
				t[p].s+=a[i],++t[p].cnt;
			}
		}
	}
	void Minus2(int p,int l,int r,int k);
	void Query2(int p,int l,int r,int &mn,int &mx,ll &s){
		Pushdown2(p);
		For(i,max(l,t[p].l),min(r,t[p].r)){
			if(a[i]>=lb&&a[i]<rb) mn=min(mn,a[i]),mx=max(mx,a[i]),s+=a[i];
		}
	}
	void Build(int p,int l,int r){
		t[p].l=l,t[p].r=r,t[p].mx=-Inf,t[p].mn=Inf;
		if(r-l+1<=Block){Pushup2(p);return;}
		int mid=(l+r)/2;
		Build(p*2,l,mid),Build(p*2+1,mid+1,r);
		Pushup(p);
	}
	void Insert(int p,int pos,int x){
		if(t[p].r-t[p].l+1<=Block){
			Pushdown2(p);
			a[pos]=x;
			Pushup2(p);
			return;
		}
		Pushdown(p);
		int mid=(t[p].l+t[p].r)/2;
		if(pos<=mid) Insert(p*2,pos,x);
		else Insert(p*2+1,pos,x);
		Pushup(p);
	}
	void Minus(int p,int l,int r,int k);
	void Query(int p,int l,int r,int &mn,int &mx,ll &s){
		if(t[p].l>r||t[p].r<l) return;
		if(l<=t[p].l&&t[p].r<=r){mn=min(mn,t[p].mn),mx=max(mx,t[p].mx),s+=t[p].s;return;}
		if(t[p].r-t[p].l+1<=Block){
			Query2(p,l,r,mn,mx,s);
			return;
		}
		Pushdown(p);
		Query(p*2,l,r,mn,mx,s);Query(p*2+1,l,r,mn,mx,s);
	}
}seg[LogBV];
void SegTree::Minus2(int p,int l,int r,int k){
	Pushdown2(p);
	For(i,max(t[p].l,l),min(t[p].r,r)){
		if(a[i]>=lb&&a[i]>k&&a[i]<rb){
			a[i]-=k;
			if(a[i]<lb) seg[Locate(a[i])].Insert(1,i,a[i]);
		}
	}
	Pushup2(p);
}
void SegTree::Minus(int p,int l,int r,int k){
	if(t[p].mx<=k) return;
	if(t[p].r-t[p].l+1<=Block){
		Minus2(p,l,r,k);
		return;
	}
	if(l<=t[p].l&&t[p].r<=r&&t[p].mn-k>=lb){
		PushMinus(p,k);
		return;
	}
	Pushdown(p);
	int mid=(t[p].l+t[p].r)/2;
	if(l<=mid) Minus(p*2,l,r,k);
	if(r>mid) Minus(p*2+1,l,r,k);
	Pushup(p);
}
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	int _mx=0;
	For(i,1,n) cin>>a[i],_mx=max(_mx,a[i]);
	for(ll cur=1;cur<=_mx;cur*=B){
		L[bcnt]=seg[bcnt].lb=cur,R[bcnt]=seg[bcnt].rb=min(ll(_mx+1),cur*B);
		++bcnt;
	}
	For(i,1,n) bel[i]=(i-1)/Block+1;
	For(i,0,bcnt-1) seg[i].Build(1,1,n);
	int last=0;
	For(_,1,m){
		int op,l,r;cin>>op>>l>>r;
		l^=last,r^=last;
		if(op==1){
			int x;cin>>x;x^=last;
			For(i,0,bcnt-1) seg[i].Minus(1,l,r,x);
		}else{
			ll s=0;int mn=Inf,mx=-Inf;
			For(i,0,bcnt-1){
				seg[i].Query(1,l,r,mn,mx,s);
			}
			cout<<s<<' '<<mn<<' '<<mx<<'\n';
			last=s%(1<<20);
		}
	}
	return 0;
}
