#include <bits/stdc++.h>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=5e4+5,Q=5e5+5,LogN=19;
int n,m,q,clen,rclen,dep;
struct Command{int p,l,r,x;}cmd[N*2],rcmd[N*2];
struct Query{int l1,r1,l2,r2,id;}qry[Q];
vector<Query> lq[LogN],rq[LogN];
set<int> mids[LogN];
void Solve(int l,int r,int vl,int vr,int _dep){
	if(l>r||vl>vr) return;
	dep=max(dep,_dep);
	int mid=(vl+vr)/2;
	mids[_dep].insert(mid);
	if(vl==vr){
		For(i,l,r) rq[_dep].push_back(qry[i]);
		return;
	}
	int p1=partition(qry+l,qry+r+1,[&mid](const Query &_q){
		return _q.r1<=mid;
	})-qry-1;
	int p2=partition(qry+p1+1,qry+r+1,[&mid](const Query &_q){
		return _q.l1<=mid;
	})-qry-1;
	For(i,p1+1,p2){
		lq[_dep].push_back({qry[i].l1,mid,qry[i].l2,qry[i].r2,qry[i].id});
		rq[_dep].push_back({mid,qry[i].r1,qry[i].l2,qry[i].r2,qry[i].id});
	}
	Solve(l,p1,vl,mid,_dep+1),Solve(p2+1,r,mid+1,vr,_dep+1);
}
struct SegTree{
	struct Node{
		int l,r;ll mx,hmx,Add,Addmx;
	}t[N*4];
	void Pushup(int p){
		t[p].mx=max(t[p*2].mx,t[p*2+1].mx);
		t[p].hmx=max(t[p*2].hmx,t[p*2+1].hmx);
	}
	void PushAdd(int p,ll k,ll k_){
		t[p].Addmx=max(t[p].Addmx,t[p].Add+k_);
		t[p].hmx=max(t[p].hmx,t[p].mx+k_);
		t[p].mx+=k;
		t[p].Add+=k;
	}
	void Pushdown(int p){
		PushAdd(p*2,t[p].Add,t[p].Addmx);
		PushAdd(p*2+1,t[p].Add,t[p].Addmx);
		t[p].Add=t[p].Addmx=0;
	}
	void Build(int p,int l,int r){
		t[p].l=l,t[p].r=r;
		if(l==r) return;
		int mid=(l+r)/2;
		Build(p*2,l,mid),Build(p*2+1,mid+1,r);
	}
	void Add(int p,int l,int r,ll k){
		if(l<=t[p].l&&t[p].r<=r){
			PushAdd(p,k,max(k,0LL));return;
		}
		Pushdown(p);
		int mid=(t[p].l+t[p].r)/2;
		if(l<=mid) Add(p*2,l,r,k);
		if(r>mid) Add(p*2+1,l,r,k);
		Pushup(p);
	}
	ll Query(int p,int l,int r){
		if(l<=t[p].l&&t[p].r<=r) return t[p].hmx;
		Pushdown(p);
		int mid=(t[p].l+t[p].r)/2;
		ll res=0;
		if(l<=mid) res=max(res,Query(p*2,l,r));
		if(r>mid) res=max(res,Query(p*2+1,l,r));
		return res;
	}
}seg;
ll ans[Q];
int main(){
	ios::sync_with_stdio(false),cin.tie(nullptr);
	cin>>n>>m>>q;
	For(i,1,m){
		int l1,r1,l2,r2,x;cin>>l1>>l2>>r1>>r2>>x;
		cmd[++clen]={l1,l2,r2,x};
		cmd[++clen]={r1+1,l2,r2,-x};
		rcmd[++rclen]={r1,l2,r2,x};
		rcmd[++rclen]={l1-1,l2,r2,-x};
	}
	sort(cmd+1,cmd+clen+1,[](const Command &c1,const Command &c2){
		return c1.p==c2.p?c1.x<c2.x:c1.p<c2.p;
	});
	sort(rcmd+1,rcmd+rclen+1,[](const Command &c1,const Command &c2){
		return c1.p==c2.p?c1.x>c2.x:c1.p<c2.p;
	});
	For(i,1,q){
		cin>>qry[i].l1>>qry[i].l2>>qry[i].r1>>qry[i].r2;
		qry[i].id=i;
	}
	Solve(1,q,1,n,1);
	seg.Build(1,1,n);
	auto Cmp=[](const Query &q1,const Query &q2){return q1.r1<q2.r1;};
	const ll C=2e13;
	For(_dep,1,dep){
		sort(rq[_dep].begin(),rq[_dep].end(),Cmp);
		For(i,1,N*4-1){
			seg.t[i].mx=seg.t[i].hmx=seg.t[i].Add=seg.t[i].Addmx=0;
		}
		ll cur=0;
		for(int i=1,cp=1,qp=0;i<=n;++i){
			while(cp<=clen&&cmd[cp].p==i){
				seg.Add(1,cmd[cp].l,cmd[cp].r,cmd[cp].x);
				++cp;
			}
			if(mids[_dep].count(i)){
				cur+=C;
				seg.Add(1,1,n,C);
			}
			while(qp<int(rq[_dep].size())&&rq[_dep][qp].r1==i){
				const auto &x=rq[_dep][qp];
				ans[x.id]=max(ans[x.id],seg.Query(1,x.l2,x.r2)-cur);
				++qp;
			}
		}
	}
	auto Cmp2=[](const Query &q1,const Query &q2){return q1.l1>q2.l1;};
	For(_dep,1,dep){
		sort(lq[_dep].begin(),lq[_dep].end(),Cmp2);
		For(i,1,N*4-1){
			seg.t[i].mx=seg.t[i].hmx=seg.t[i].Add=seg.t[i].Addmx=0;
		}
		ll cur=0;
		for(int i=n,cp=rclen,qp=0;i>=1;--i){
			while(cp>=1&&rcmd[cp].p==i){
				seg.Add(1,rcmd[cp].l,rcmd[cp].r,rcmd[cp].x);
				--cp;
			}
			if(mids[_dep].count(i)){
				cur+=C;
				seg.Add(1,1,n,C);
			}
			while(qp<int(lq[_dep].size())&&lq[_dep][qp].l1==i){
				const auto &x=lq[_dep][qp];
				ans[x.id]=max(ans[x.id],seg.Query(1,x.l2,x.r2)-cur);
				++qp;
			}
		}
	}
	For(i,1,q) cout<<ans[i]<<'\n';
	#ifdef zyz
	Debug("Elapsed time: %dms\n",int(clock()));
	#endif
	return 0;
}
