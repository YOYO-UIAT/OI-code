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
const int N=1e5+5,V=4e4+5;
const double Eps=1e-8;
struct Seg{
	int id,x0,y0,x1,y1;
	double Eval(int x) const{
		if(x0==x1) return max(y0,y1);
		else return y0+double(y1-y0)/(x1-x0)*(x-x0);
	}
};
struct SegTree{
	struct Node{
		int l,r;Seg s;
	}t[V*4];
	void Build(int p,int l,int r){
		t[p].l=l,t[p].r=r;
		if(l==r) return;
		int mid=(l+r)/2;
		Build(p*2,l,mid),Build(p*2+1,mid+1,r);
	}
	void InsertNode(int p,const Seg &s){
		int lcomp=(s.Eval(t[p].l)>=t[p].s.Eval(t[p].l));
		int rcomp=(s.Eval(t[p].r)>=t[p].s.Eval(t[p].r));
		int mid=(t[p].l+t[p].r)/2,mcomp=(s.Eval(mid)>=t[p].s.Eval(mid));
		if(lcomp&&rcomp){t[p].s=s;return;}
		if(!lcomp&&!rcomp) return;
		if(mcomp){
			InsertNode(lcomp?p*2+1:p*2,t[p].s);
			t[p].s=s;
		}else{
			InsertNode(lcomp?p*2:p*2+1,s);
		}
	}
	void Insert(int p,const Seg &s){
		if(s.x0<=t[p].l&&t[p].r<=s.x1){InsertNode(p,s);return;}
		int mid=(t[p].l+t[p].r)/2;
		if(s.x0<=mid) Insert(p*2,s);
		if(s.x1>mid) Insert(p*2+1,s);
	}
	Seg Query(int p,int pos){
		if(t[p].l==t[p].r) return t[p].s;
		int mid=(t[p].l+t[p].r)/2;
		Seg s=Query(pos<=mid?p*2:p*2+1,pos);
		double x=s.Eval(pos),y=t[p].s.Eval(pos);
		if(abs(x-y)<Eps) return s.id<t[p].s.id?s:t[p].s;
		else return x>y?s:t[p].s;
	}
}seg;
int m;
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>m;
	seg.Build(1,1,V-1);
	int last=0;
	auto &&F=[&last](int &x,int mod){x=(x+last-1)%mod+1;};
	int cnt=0;
	For(_,1,m){
		int op;cin>>op;
		if(op==0){
			int k;cin>>k;F(k,39989);
			cout<<(last=seg.Query(1,k).id)<<'\n';
		}else{
			int x0,y0,x1,y1;cin>>x0>>y0>>x1>>y1;
			F(x0,39989),F(y0,1e9),F(x1,39989),F(y1,1e9);
			if(x1<x0) swap(x0,x1),swap(y0,y1);
			seg.Insert(1,Seg{++cnt,x0,y0,x1,y1});
		}
	}
	return 0;
}
