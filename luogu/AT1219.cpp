#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=1e5+5,SqrtN=320;
int n,q,a[N],disca[N],_cnt[N],orig[N];
vector<int> disc[N];
int bsiz,bel[N],sum[SqrtN],arr[N],cnt[N],L[N],R[N];
void Modify(int p,int k){
	p=disca[p];
	if(cnt[p]){
		int pre=disc[p][cnt[p]];
		--sum[bel[pre]],--arr[pre];
	}
	cnt[p]+=k;
	int cur=disc[p][cnt[p]];
	++sum[bel[cur]],++arr[cur];
}
struct Query{
	static int siz;
	int l,r,i;
	bool operator<(const Query &x){
		if(l/siz!=x.l/siz) return l<x.l;
		return r==x.r?0:((r<x.r)^((l/siz)&1));
	}
}qry[N];
int Query::siz;ll ans[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>q;
	map<int,int> mp;
	int tot=0;
	vector<ll> _disc;
	For(i,1,n){
		cin>>a[i];
		auto it=mp.find(a[i]);
		if(it==mp.end()) it=mp.insert({a[i],++tot}).first,orig[tot]=a[i];
		disca[i]=it->second;
		++_cnt[disca[i]];
		_disc.push_back(1LL*a[i]*_cnt[disca[i]]);
	}
	sort(_disc.begin(),_disc.end());
	For(i,1,tot){
		disc[i].resize(_cnt[i]+1);
		For(j,1,_cnt[i]){
			disc[i][j]=lower_bound(_disc.begin(),_disc.end(),1LL*orig[i]*j)-_disc.begin()+1;
		}
	}
	bsiz=sqrt(n);
	For(i,1,n) bel[i]=(i-1)/bsiz+1,R[bel[i]]=i;
	Dec(i,n,1) L[bel[i]]=i;
	Query::siz=n/sqrt(q);
	For(i,1,q){
		cin>>qry[i].l>>qry[i].r;qry[i].i=i;
	}
	sort(qry+1,qry+q+1);
	for(int i=1,l=1,r=0;i<=q;++i){
		if(qry[i].l==qry[i].r){
			ans[qry[i].i]=a[qry[i].l];continue;
		}
		while(l>qry[i].l) --l,Modify(l,1);
		while(r<qry[i].r) ++r,Modify(r,1);
		while(l<qry[i].l) Modify(l,-1),++l;
		while(r>qry[i].r) Modify(r,-1),--r;
		int bl=0;
		Dec(j,bel[n],1){
			if(sum[j]){bl=j;break;}
		}
		Dec(j,R[bl],L[bl]){
			if(arr[j]){ans[qry[i].i]=_disc[j-1];break;}
		}
	}
	For(i,1,q) cout<<ans[i]<<'\n';
	return 0;
}
