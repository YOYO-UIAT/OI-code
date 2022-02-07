#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
typedef long long ll;
const int N=1e6+5,K=12;
int n,m,a[N];
struct Fenwick{
	int t[N];
	void Add(int p,int k){for(;p<=n;p+=p&-p) t[p]+=k;}
	void Add(int l,int r,int k){Add(l,k),Add(r+1,-k);}
	int Query(int p){int res=0;for(;p;p-=p&-p) res+=t[p];return res;}
}bit[K];
struct Qry{int l,r,i;}q[N];
vector<int> occ[N];
int Pre(int p,int x){
	while(occ[x].size()>1&&occ[x].rbegin()[1]<p) occ[x].pop_back();
	return occ[x].back();
}
int vis[N],ans[N][K];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	int mx=0;
	For(i,1,n) cin>>a[i],mx=max(mx,a[i]),occ[a[i]].push_back(i);
	For(i,1,mx) reverse(occ[i].begin(),occ[i].end()),occ[i].push_back(0);
	For(i,1,m) cin>>q[i].l>>q[i].r,q[i].i=i;
	sort(q+1,q+m+1,[](const Qry &q1,const Qry &q2){return q1.r<q2.r;});
	int p=1;
	For(r,1,n){
		int l=Pre(r,a[r]);
		vector<int> pos{l,r};
		For(x,max(1,a[r]-11),min(mx,a[r]+11)){
			int pre=Pre(r,x);
			if(pre>l) pos.push_back(pre);
		}
		sort(pos.begin(),pos.end(),greater<int>());
		for(auto it=pos.begin();next(it)!=pos.end();++it){
			vis[a[*it]]=r;
			int len1=0,len2=0;
			for(int i=a[r]-1;i>=1&&vis[i]==r;--i) ++len1;
			for(int i=a[r]+1;i<=mx&&vis[i]==r;++i) ++len2;
			if(len1+len2+1<=10) bit[len1+len2+1].Add(*next(it)+1,*it,1);
			if(len1&&len1<=10) bit[len1].Add(*next(it)+1,*it,-1);
			if(len2&&len2<=10) bit[len2].Add(*next(it)+1,*it,-1);
		}
		for(;p<=m&&q[p].r==r;++p){
			For(i,1,10) ans[q[p].i][i]=bit[i].Query(q[p].l)%10;
		}
	}
	For(i,1,m){
		For(j,1,10) cout.put(ans[i][j]+48);
		cout.put('\n');
	}
	return 0;
}
