#include <bits/stdc++.h>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=3e5+5,LogN=20,Mod=1e9+7,Inf=0x3f3f3f3f,Thr=8e6+5;
int n,m,a[N];
struct Treap{
	struct Node{
		int ls,rs,s,v,siz,Emp;bool Rev;int Add;
	}t[Thr];
	int root,tot;
	int New(int v){
		t[++tot]={0,0,v,v,1,-Inf,0,0};
		return tot;
	}
	void Pushup(int p){
		t[p].s=((t[t[p].ls].s+t[t[p].rs].s)%Mod+t[p].v)%Mod;
		t[p].siz=t[t[p].ls].siz+t[t[p].rs].siz+1;
	}
	void PushEmp(int p,int k){
		t[p].s=1LL*t[p].siz*k%Mod,t[p].v=t[p].Emp=k;
		t[p].Add=0;
	}
	void PushRev(int p){
		swap(t[p].ls,t[p].rs);
		t[p].Rev^=1;
	}
	void PushAdd(int p,int k){
		t[p].v=(t[p].v+k)%Mod,t[p].s=(t[p].s+1LL*t[p].siz*k)%Mod;
		t[p].Add=(t[p].Add+k)%Mod;
	}
	void Pushdown(int p){
		if(t[p].Emp!=-Inf||t[p].Add||t[p].Rev){
			if(t[p].ls) ++tot,t[tot]=t[t[p].ls],t[p].ls=tot;
			if(t[p].rs) ++tot,t[tot]=t[t[p].rs],t[p].rs=tot;
		}
		if(t[p].Emp!=-Inf){
			PushEmp(t[p].ls,t[p].Emp),PushEmp(t[p].rs,t[p].Emp);
			t[p].Emp=-Inf;
		}
		if(t[p].Add){
			PushAdd(t[p].ls,t[p].Add),PushAdd(t[p].rs,t[p].Add);
			t[p].Add=0;
		}
		if(t[p].Rev) PushRev(t[p].ls),PushRev(t[p].rs),t[p].Rev=0;
	}
	int Build(int l,int r){
		if(l>r) return 0;
		int mid=(l+r)/2,u=New(a[mid]);
		t[u].ls=Build(l,mid-1),t[u].rs=Build(mid+1,r);
		Pushup(u);
		return u;
	}
	void Split(int p,int k,int &x,int &y){
		if(!p){x=y=0;return;}
		Pushdown(p);
		int siz=t[t[p].ls].siz;
		if(siz>=k){
			y=++tot,t[y]=t[p];
			Split(t[y].ls,k,x,t[y].ls);
			Pushup(y);
		}else{
			x=++tot,t[x]=t[p];
			Split(t[x].rs,k-siz-1,t[x].rs,y);
			Pushup(x);
		}
	}
	int Merge(int p,int q){
		if(!p||!q) return p+q;
		Pushdown(p),Pushdown(q);
		if(rand()%(t[p].siz+t[q].siz)<t[p].siz){
			int u=++tot;t[u]=t[p];
			t[u].rs=Merge(t[u].rs,q);
			Pushup(u);return u;
		}else{
			int u=++tot;t[u]=t[q];
			t[u].ls=Merge(p,t[u].ls);
			Pushup(u);return u;
		}
	}
	int QSum(int l,int r){
		int x,y,z;Split(root,l-1,x,y),Split(y,r-l+1,y,z);
		int res=t[y].s;
		root=Merge(Merge(x,y),z);
		return res;
	}
	void Emplace(int l,int r,int k){
		int x,y,z;Split(root,l-1,x,y),Split(y,r-l+1,y,z);
		PushEmp(y,k);
		root=Merge(Merge(x,y),z);
	}
	void Add(int l,int r,int k){
		int x,y,z;Split(root,l-1,x,y),Split(y,r-l+1,y,z);
		PushAdd(y,k);
		root=Merge(Merge(x,y),z);
	}
	void Copy(int l1,int r1,int l2,int r2){
		int _a,b,c,d,e,sw=0;
		if(l1>l2) swap(l1,l2),swap(r1,r2),sw=1;
		Split(root,l1-1,_a,b),Split(b,r1-l1+1,b,c);
		Split(c,l2-r1-1,c,d),Split(d,r2-l2+1,d,e);
		if(!sw) d=++tot,t[d]=t[b];
		else b=++tot,t[b]=t[d];
		root=Merge(Merge(Merge(Merge(_a,b),c),d),e);
	}
	void Swap(int l1,int r1,int l2,int r2){
		int _a,b,c,d,e;
		if(l1>l2) swap(l1,l2),swap(r1,r2);
		Split(root,l1-1,_a,b),Split(b,r1-l1+1,b,c);
		Split(c,l2-r1-1,c,d),Split(d,r2-l2+1,d,e);
		root=Merge(Merge(Merge(_a,d),c),Merge(b,e));
	}
	void Reverse(int l,int r){
		int x,y,z;Split(root,l-1,x,y),Split(y,r-l+1,y,z);
		PushRev(y);
		root=Merge(Merge(x,y),z);
	}
	int cur;
	void Dfs(int p){
		Pushdown(p);
		if(t[p].ls) Dfs(t[p].ls);
		a[++cur]=t[p].v;
		if(t[p].rs) Dfs(t[p].rs);
	}
}tr;
int main(){
	ios::sync_with_stdio(false),cin.tie(nullptr);
	cin>>n>>m;
	For(i,1,n) cin>>a[i];
	tr.root=tr.Build(1,n);
	int last=0;
	For(_,1,m){
		int op,l,r;cin>>op>>l>>r;
		l^=last,r^=last;
		if(op==1) cout<<(last=tr.QSum(l,r))<<'\n';
		else if(op==2){int x;cin>>x;x^=last;tr.Emplace(l,r,x);}
		else if(op==3){int x;cin>>x;x^=last;tr.Add(l,r,x);}
		else if(op==4){int L,R;cin>>L>>R;L^=last,R^=last;tr.Copy(l,r,L,R);}
		else if(op==5){int L,R;cin>>L>>R;L^=last,R^=last;tr.Swap(l,r,L,R);}
		else tr.Reverse(l,r);
		if(tr.tot>=Thr-3e5){
			tr.cur=0;tr.Dfs(tr.root);
			tr.root=tr.tot=0;tr.root=tr.Build(1,n);
		}
	}
	tr.cur=0;tr.Dfs(tr.root);
	For(i,1,n) cout<<a[i]<<' ';
	#ifdef zyz
		Debug("Elapsed time: %dms\n",int(clock()));
	#endif
	return 0;
}
