#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=1e6+5;
int n,m,a[N];
struct Bit{
	int t[N];ll s[N];
	void Add(int p,int k,int real){
		ll x=1LL*k*real;
		for(;p<=m;p+=p&-p) s[p]+=x,t[p]+=k;
	}
	ll QSum(int p){
		ll res=0;
		for(;p;p-=p&-p) res+=s[p];
		return res;
	}
	int QCnt(int p){
		int res=0;
		for(;p;p-=p&-p) res+=t[p];
		return res;
	}
}bit;
struct Op{
	char op[2];int x,y;
}b[N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m;
	vector<int> disc;
	For(_,1,m){
		cin>>b[_].op>>b[_].x>>b[_].y;
		disc.push_back(b[_].y);
	}
	sort(disc.begin(),disc.end());
	For(_,1,m){
		int x=b[_].x,y=b[_].y;
		y=lower_bound(disc.begin(),disc.end(),y)-disc.begin()+1;
		if(b[_].op[0]=='U'){
			if(a[x]) bit.Add(a[x],-1,disc[a[x]-1]);
			bit.Add(y,1,disc[y-1]);
			a[x]=y;
		}else{
			int cnt=bit.QCnt(m)-bit.QCnt(y-1);
			cout<<(bit.QSum(y-1)>=1LL*(x-cnt)*disc[y-1]?"TAK":"NIE")<<'\n';
		}
	}
	return 0;
}
