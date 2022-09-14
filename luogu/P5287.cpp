#include <bits/stdc++.h>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=1e5+5,Mod=998244353;
int n,len,kmp[N],slen[N];ll ans[N];
pair<int,char> s[N];
vector<int> e[N];
struct Oper{int tp,x;char c;}op[N];
ll Sum(ll x){return (x+1)*x/2%Mod;}
void Dfs(int u,int fa){
	ans[u]+=ans[fa];
	if(op[u].tp==1){
		s[++len]={op[u].x,op[u].c},slen[len]=slen[len-1]+op[u].x;
		if(len==1) ans[u]=Sum(s[len].first-1);
		else{
			int j=kmp[len-1],cur=0,pre=0;
			while(true){
				if(s[j+1].second==s[len].second){
					int siz=min(s[j+1].first,s[len].first);
					if(siz>cur) (ans[u]+=(Sum(siz+slen[j])-Sum(cur+slen[j])+Mod))%=Mod,cur=siz;
				}
				if(!j||s[j+1]==s[len]) break;
				if(j-kmp[j]==pre) j=j%pre+pre;
				pre=j-kmp[j],j=kmp[j];
			}
			if(s[j+1]==s[len]) ++j;
			if(!j&&cur<s[len].first&&s[1].second==s[len].second){
				(ans[u]+=s[1].first*(s[len].first-cur))%=Mod;
			}
			if(!j&&s[1].first<s[len].first&&s[1].second==s[len].second) ++j;
			kmp[len]=j;
		}
	}
	for(int v:e[u]) Dfs(v,u);
	if(op[u].tp==1) --len;
}
int main(){
	ios::sync_with_stdio(false),cin.tie(nullptr);
	cin>>n;
	For(i,1,n){
		cin>>op[i].tp>>op[i].x;
		if(op[i].tp==1) cin>>ws>>op[i].c,e[i-1].push_back(i);
		else e[op[i].x].push_back(i);
	}
	Dfs(0,0);
	For(i,1,n) cout<<ans[i]<<'\n';
	#ifdef zyz
		Debug("Elapsed time: %dms\n",int(clock()));
	#endif
	return 0;
}
