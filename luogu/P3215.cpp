#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=2e5+5;
mt19937 eng(chrono::system_clock::now().time_since_epoch().count());
struct Treap{
	struct Node{
		int ls,rs,typ,siz,key,lc,rc,slc,src,ilc,irc,islc,isrc;
		int Rep,Swap,Inv;
	}t[N];
	int tot=0,root;
	int New(){
		t[++tot].key=eng(),t[tot].Rep=-1,t[tot].siz=1;
		return tot;
	}
	void Pushup(int p){
		int ls=t[p].ls,rs=t[p].rs,L=1-t[p].typ,R=t[p].typ;
		t[p].lc=t[ls].lc+max(0,t[rs].lc+R-t[ls].rc-L);
		t[p].rc=t[rs].rc+max(0,t[ls].rc+L-t[rs].lc-R);
		t[p].slc=t[rs].slc+max(0,t[ls].slc+R-t[rs].src-L);
		t[p].src=t[ls].src+max(0,t[rs].src+L-t[ls].slc-R);
		t[p].ilc=t[ls].ilc+max(0,t[rs].ilc+L-t[ls].irc-R);
		t[p].irc=t[rs].irc+max(0,t[ls].irc+R-t[rs].ilc-L);
		t[p].islc=t[rs].islc+max(0,t[ls].islc+L-t[rs].isrc-R);
		t[p].isrc=t[ls].isrc+max(0,t[rs].isrc+R-t[ls].islc-L);
		t[p].siz=t[ls].siz+t[rs].siz+1;
	}
	void PushRep(int p,int k){
		t[p].typ=k;
		t[p].lc=t[p].slc=t[p].irc=t[p].isrc=k*t[p].siz;
		t[p].rc=t[p].src=t[p].ilc=t[p].islc=(1-k)*t[p].siz;
		t[p].Rep=k,t[p].Swap=t[p].Inv=0;
	}
	void PushSwap(int p){
		swap(t[p].lc,t[p].slc),swap(t[p].rc,t[p].src);
		swap(t[p].ilc,t[p].islc),swap(t[p].irc,t[p].isrc);
		swap(t[p].ls,t[p].rs);
		t[p].Swap^=1;
	}
	void PushInv(int p){
		t[p].typ^=1;
		swap(t[p].lc,t[p].ilc),swap(t[p].rc,t[p].irc);
		swap(t[p].slc,t[p].islc),swap(t[p].src,t[p].isrc);
		t[p].Inv^=1;
	}
	void Pushdown(int p){
		if(~t[p].Rep) PushRep(t[p].ls,t[p].Rep),PushRep(t[p].rs,t[p].Rep),t[p].Rep=-1;
		if(t[p].Swap) PushSwap(t[p].ls),PushSwap(t[p].rs),t[p].Swap=0;
		if(t[p].Inv) PushInv(t[p].ls),PushInv(t[p].rs),t[p].Inv=0;
	}
	void Split(int p,int k,int &x,int &y){
		if(!p){x=y=0;return;}
		Pushdown(p);
		int siz=t[t[p].ls].siz;
		if(siz>=k) y=p,Split(t[p].ls,k,x,t[p].ls);
		else x=p,Split(t[p].rs,k-siz-1,t[p].rs,y);
		Pushup(p);
	}
	int Merge(int p,int q){
		Pushdown(p),Pushdown(q);
		if(!p||!q) return p+q;
		if(t[p].key<t[q].key){
			t[p].rs=Merge(t[p].rs,q);
			Pushup(p);return p;
		}else{
			t[q].ls=Merge(p,t[q].ls);
			Pushup(q);return q;
		}
	}
}tr;
int n,m;char s[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m>>(s+1);
	For(i,1,n){
		int u=tr.New();
		tr.PushRep(u,s[i]==')'),tr.t[u].Rep=-1;
		tr.root=tr.Merge(tr.root,u);
	}
	For(_,1,m){
		char op[10];int x,y;
		cin>>op>>x>>y;
		int u,v,w;
		tr.Split(tr.root,x-1,u,v);
		tr.Split(v,y-x+1,v,w);
		if(op[0]=='R'){
			char c[3];cin>>c;
			tr.PushRep(v,c[0]==')');
		}else if(op[0]=='S'){
			tr.PushSwap(v);
		}else if(op[0]=='I'){
			tr.PushInv(v);
		}else{
			// Debug("lc=%d,rc=%d\n",tr.t[v].lc,tr.t[v].rc);
			cout<<(tr.t[v].lc+1)/2+(tr.t[v].rc+1)/2<<'\n';
		}
		tr.root=tr.Merge(u,tr.Merge(v,w));
	}
	return 0;
}
