#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
typedef long long ll;
struct Frac{
	ll a,b;
	Frac(ll _a=0,ll _b=1):a(_a),b(_b){}
	Frac operator+(const Frac &x) const{
		ll g=__gcd(b,x.b),k=a*(x.b/g)+x.a*(b/g),y=b/g*x.b;
		return {k,y};
	}
	Frac operator-() const{return {-a,b};}
	Frac operator-(const Frac &x) const{return (*this)+(-x);}
	bool operator<(const Frac &x) const{
		return a*x.b<b*x.a;
	}
	bool operator>(const Frac &x) const{
		return a*x.b>b*x.a;
	}
};
int n,m,k;
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>m>>k;
	Frac cur;
	priority_queue<Frac> q;
	For(i,2,n) q.push({i-1,i});
	while(k--){
		cur=q.top();q.pop();
		ll a=cur.a;
		if(a>1){
			--a;
			while(a>1&&__gcd(a,cur.b)>1) --a;
			if(a>=1) q.push({a,cur.b});
		}
	}
	cout<<cur.a<<'/'<<cur.b<<' ';
	Frac mn(10000000),ans;
	for(ll b=2;b<=m;++b){
		ll a=cur.a*b/cur.b;
		if(a>=1&&a<b&&__gcd(a,b)==1){
			Frac res=cur-Frac(a,b);
			if(res<mn) mn=res,ans=Frac(a,b);
		}
		++a;
		if(a>=1&&a<b&&__gcd(a,b)==1){
			Frac res=Frac(a,b)-cur;
			if(res<mn) mn=res,ans=Frac(a,b);
		}
	}
	cout<<ans.a<<'/'<<ans.b;
	return 0;
}
