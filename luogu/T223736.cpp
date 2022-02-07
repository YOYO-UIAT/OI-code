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
const int N=5005;
const ll Inf=0x3f3f3f3f3f3f3f3fLL;
int n;ll c,a[N],b[N],f[N][N];
int main(){
	#ifndef zyz
	ios::sync_with_stdio(false),cin.tie(nullptr);
	#endif
	cin>>n>>c;
	For(i,1,n) cin>>a[i];
	For(i,1,n) cin>>b[i];
	memset(f,0x3f,sizeof f);
	f[0][0]=0;
	For(i,1,n){
		For(j,0,i){
			if(j) f[i][j]=f[i-1][j-1]+b[i];
			f[i][j]=min(f[i][j],f[i-1][j]+a[i]+c*j);
		}
	}
	ll ans=Inf;
	For(i,0,n) ans=min(ans,f[n][i]);
	cout<<ans;
	return 0;
}
