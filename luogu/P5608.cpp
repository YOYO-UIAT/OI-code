#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
using pii=pair<int,int>;
const int N=1e5+5,Mod=1e9+7;
long long Pow(long long _base,long long _pow){
	long long _res=1;
	while(_pow){
		if(_pow&1) _res=_res*_base%Mod;
		_pow>>=1,_base=_base*_base%Mod;
	}
	return _res;
}
int n,m;ll a[N],sym[N];
struct SegTree{
	struct Node{
		int l,r,rsym,rp,lp,Num,Sym=-1;ll s,rs,ls,sum,mul,lv,rv;
		vector<pii> *vec;
	}t[N<<2];
	void Oper(Node &p,const Node &ls,const Node &rs){
		if(!ls.l){p=rs;return;}
		if(!rs.l){p=ls;return;}
		p.l=ls.l,p.r=rs.r;
		p.rsym=rs.rsym;
		p.sum=(ls.sum+rs.sum)%Mod;
		p.mul=ls.mul*rs.mul%Mod;
		p.lv=ls.lv,p.rv=rs.rv;
		if(ls.rsym)
			p.s=(ls.s-ls.rs+ls.rs*rs.ls%Mod+rs.s-rs.ls+Mod*2)%Mod;
		else
			p.s=(ls.s+rs.s)%Mod;
		if(rs.rp==rs.l&&ls.rsym)
			p.rp=ls.rp,p.rs=rs.rs*ls.rs%Mod;
		else
			p.rp=rs.rp,p.rs=rs.rs;
		if(ls.lp==ls.r&&ls.rsym)
			p.lp=rs.lp,p.ls=ls.ls*rs.ls%Mod;
		else
			p.lp=ls.lp,p.ls=ls.ls;
	}
	void OperVec(Node &p,const Node &ls,const Node &rs){
		const auto &lvec=ls.vec,&rvec=rs.vec;
		p.vec->clear();
		p.vec->reserve(lvec->size()+rvec->size()+1);
		auto jt=rvec->begin();
		for(auto it=lvec->begin();it!=lvec->end();++it){
			while(jt!=rvec->end()&&jt->first<it->first){
				p.vec->emplace_back(*jt);
				++jt;
			}
			if(jt!=rvec->end()&&jt->first==it->first)
				p.vec->emplace_back(it->first,it->second+jt->second),++jt;
			else
				p.vec->emplace_back(*it);
		}
		while(jt!=rvec->end()) p.vec->emplace_back(*jt),++jt;
		if(ls.rsym){
			int len1=ls.r-ls.rp+1,len2=rs.lp-rs.l+1;
			auto it1=lower_bound(p.vec->begin(),p.vec->end(),pii{len1,0});
			if(!--it1->second) p.vec->erase(it1);
			auto it2=lower_bound(p.vec->begin(),p.vec->end(),pii{len2,0});
			if(!--it2->second) p.vec->erase(it2);
			auto it=lower_bound(p.vec->begin(),p.vec->end(),pii{len1+len2,0});
			if(it==p.vec->end()||it->first!=len1+len2){
				p.vec->insert(it,pii{len1+len2,1});
			}else{
				++it->second;
			}
		}
	}
	void PushNum(int p,ll k){
		t[p].lv=t[p].rv=k;
		t[p].ls=Pow(k,t[p].lp-t[p].l+1);
		t[p].rs=Pow(k,t[p].r-t[p].rp+1);
		t[p].sum=(t[p].r-t[p].l+1)*k%Mod;
		t[p].mul=Pow(k,t[p].r-t[p].l+1);
		t[p].s=0;
		ll cur=1;int _p=0;
		for(const auto &x:*t[p].vec){
			(cur*=Pow(k,x.first-_p))%=Mod,_p=x.first;
			(t[p].s+=x.second*cur)%=Mod;
		}
		t[p].Num=k;
	}
	void PushSym(int p,int k){
		t[p].rsym=k;
		if(!k){
			t[p].vec->clear();
			t[p].vec->emplace_back(1,t[p].r-t[p].l+1);
			t[p].rp=t[p].r,t[p].lp=t[p].l;
			t[p].rs=t[p].rv,t[p].ls=t[p].lv;
			t[p].s=t[p].sum;
		}else{
			t[p].vec->clear();
			t[p].vec->emplace_back(t[p].r-t[p].l+1,1);
			t[p].rp=t[p].l,t[p].lp=t[p].r;
			t[p].rs=t[p].ls=t[p].s=t[p].mul;
		}
		t[p].Sym=k;
	}
	void Pushdown(int p){
		if(t[p].Sym!=-1){
			PushSym(p*2,t[p].Sym),PushSym(p*2+1,t[p].Sym),t[p].Sym=-1;
		}
		if(t[p].Num){
			PushNum(p*2,t[p].Num),PushNum(p*2+1,t[p].Num),t[p].Num=0;
		}
	}
	void Build(int p,int l,int r){
		t[p].l=l,t[p].r=r,t[p].vec=new vector<pii>();
		if(l==r){
			t[p].rsym=sym[l],t[p].rp=t[p].lp=l,t[p].s=t[p].rs=t[p].ls=t[p].sum=t[p].mul=a[l];
			t[p].lv=t[p].rv=a[l];
			t[p].vec->emplace_back(1,1);
			return;
		}
		int mid=(l+r)/2;
		Build(p*2,l,mid),Build(p*2+1,mid+1,r);
		Oper(t[p],t[p*2],t[p*2+1]);
		OperVec(t[p],t[p*2],t[p*2+1]);
	}
	void ModNum(int p,int l,int r,ll k){
		if(l<=t[p].l&&t[p].r<=r){PushNum(p,k);return;}
		Pushdown(p);
		int mid=(t[p].l+t[p].r)/2;
		if(l<=mid) ModNum(p*2,l,r,k);
		if(r>mid) ModNum(p*2+1,l,r,k);
		Oper(t[p],t[p*2],t[p*2+1]);
	}
	void ModSym(int p,int l,int r,int k){
		if(l<=t[p].l&&t[p].r<=r){PushSym(p,k);return;}
		Pushdown(p);
		int mid=(t[p].l+t[p].r)/2;
		if(l<=mid) ModSym(p*2,l,r,k);
		if(r>mid) ModSym(p*2+1,l,r,k);
		Oper(t[p],t[p*2],t[p*2+1]);
		OperVec(t[p],t[p*2],t[p*2+1]);
	}
	Node Query(int p,int l,int r){
		if(l>t[p].r||r<t[p].l) return {};
		if(l<=t[p].l&&t[p].r<=r) return t[p];
		Pushdown(p);
		Node res;
		Oper(res,Query(p*2,l,r),Query(p*2+1,l,r));
		return res;
	}
}seg;
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	For(i,1,n) cin>>a[i],a[i]%=Mod;
	For(i,1,n-1) cin>>sym[i];
	seg.Build(1,1,n);
	For(_,1,m){
		int op,l,r;cin>>op>>l>>r;
		if(op==1){
			ll x;cin>>x;x%=Mod;
			seg.ModNum(1,l,r,x);
		}else if(op==2){
			int x;cin>>x;
			seg.ModSym(1,l,r,x);
		}else{
			cout<<seg.Query(1,l,r).s<<'\n';
		}
	}
	return 0;
}
