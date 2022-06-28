#include <bits/stdc++.h>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
ll Pow(ll _base,ll _pow,const ll& _mod){
	ll _res=1;
	while(_pow){
		if(_pow&1) _res=_res*_base%_mod;
		_pow>>=1,_base=_base*_base%_mod;
	}
	return _res%_mod;
}
struct Comb{
	const int Lim,Mod;
	ll *_fac,*_ifac;
	Comb(int _lim,int _mod):Lim(_lim),Mod(_mod){
		_fac=new ll[Lim+1],_ifac=new ll[Lim+1];
		_fac[0]=1;
		for(int _i=1;_i<=Lim;++_i) _fac[_i]=_fac[_i-1]*_i%Mod;
		_ifac[Lim]=Pow(_fac[Lim],Mod-2,Mod);
		for(int _i=Lim-1;_i>=0;--_i) _ifac[_i]=_ifac[_i+1]*(_i+1)%Mod;
	}
	ll C(int _n,int _m) const{
		if(_n<_m) return 0;
		return _fac[_n]*_ifac[_m]%Mod*_ifac[_n-_m]%Mod;
	}
	~Comb(){delete[] _fac;delete[] _ifac;}
};
const int N=2e5+5,M=13,SqrtN=460,Mod=998244353;
int n,m,tp;
namespace Big{
	void Solve(){
		Comb comb(2*n,Mod);
		For(i,1,n){
			if(tp||i==n){
				ll ans=0;
				for(int k=i%(m+1);k<=i*2;k+=m+1){
					(ans+=comb.C(i*2,k))%=Mod;
				}
				(ans*=(m+1))%=Mod;
				ans=(ans-Pow(2,2*i,Mod)+Mod)%Mod;
				cout<<ans<<'\n';
			}
		}
	}
}
namespace Small{
	ll pre[SqrtN],temp[SqrtN],cur[SqrtN];
	void Solve(){
		pre[0]=1;
		For(i,1,n){
			For(j,0,m) temp[j]=(pre[j]+pre[(j+m)%(m+1)])%Mod;
			For(j,0,m) cur[j]=(temp[j]+temp[(j+m)%(m+1)])%Mod;
			ll ans=(cur[i%(m+1)]*(m+1)%Mod-Pow(2,2*i,Mod)+Mod)%Mod;
			if(tp||i==n) cout<<ans<<'\n';
			For(j,0,m) pre[j]=cur[j];
		}
	}
}
int main(){
	ios::sync_with_stdio(false),cin.tie(nullptr);
	freopen("ring.in","r",stdin);
	freopen("ring.out","w",stdout);
	cin>>n>>m>>tp;
	if(m>=sqrt(n)) Big::Solve();
	else Small::Solve();
	#ifdef zyz
		Debug("Elapsed time: %dms\n",int(clock()));
	#endif
	return 0;
}
