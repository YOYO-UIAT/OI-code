#include"koala.h"
#include <bits/stdc++.h>
using namespace std;
#define For(Ti,Ta,Tb) for(int Ti=(Ta);Ti<=(Tb);++Ti)
#define Dec(Ti,Ta,Tb) for(int Ti=(Ta);Ti>=(Tb);--Ti)
#define Debug(...) fprintf(stderr,__VA_ARGS__)
using ll=long long;
const int N=105;
int A[N],B[N];
void Play(){playRound(A,B);}
int minValue(int n,[[maybe_unused]] int m){
	A[0]=1;
	For(i,1,n-1) A[i]=0;
	Play();
	For(i,0,n-1) if(B[i]<=A[i]) return i;
	return 0;
}
int maxValue(int n,[[maybe_unused]] int m){
	static int cur[N];
	For(i,0,n-1) A[i]=1,cur[i]=0;
	Play();
	For(i,0,n-1) cur[i]=bool(B[i]>1);
	For(i,0,n-1) A[i]=cur[i]*2;
	Play();
	For(i,0,n-1) cur[i]&=bool(B[i]>2);
	For(i,0,n-1) A[i]=cur[i]*4;
	Play();
	For(i,0,n-1) cur[i]&=bool(B[i]>4);
	For(i,0,n-1) A[i]=cur[i]*11;
	Play();
	For(i,0,n-1){
		cur[i]&=bool(B[i]>11);
		if(cur[i]) return i;
	}
	return 0;
}
int greaterValue(int n,[[maybe_unused]] int m){
	int l=1,r=14;
	while(l<=r){
		int mid=(l+r)/2;
		A[0]=A[1]=mid;
		For(i,2,n-1) A[i]=0;
		Play();
		bool s[2]{B[0]>mid,B[1]>mid};
		if(s[0]!=s[1]) return !s[0];
		if(s[0]&&s[1]) l=mid+1;
		else r=mid-1;
	}
	return 0;
}
map<pair<int,int>,int> mp{{{1,2},1},{{1,4},1},{{1,7},1},{{1,13},1},{{1,25},1},{{1,50},1},{{1,100},1},{{3,4},1},{{5,7},1},{{6,7},2},{{8,10},2},{{8,13},2},{{9,10},2},{{11,13},2},{{12,13},3},{{14,15},3},{{14,19},2},{{14,25},2},{{16,17},3},{{16,19},3},{{18,19},3},{{20,22},3},{{20,25},3},{{21,22},4},{{23,25},3},{{24,25},4},{{26,27},4},{{26,30},3},{{26,38},2},{{26,50},2},{{28,30},3},{{29,30},4},{{31,32},5},{{31,34},4},{{31,38},3},{{33,34},5},{{35,36},5},{{35,38},4},{{37,38},5},{{39,41},4},{{39,44},4},{{39,50},3},{{40,41},5},{{42,44},4},{{43,44},5},{{45,47},4},{{45,50},4},{{46,47},6},{{48,50},4},{{49,50},6},{{51,52},6},{{51,54},5},{{51,60},3},{{51,75},2},{{51,100},2},{{53,54},6},{{55,57},5},{{55,60},4},{{56,57},6},{{58,60},5},{{59,60},6},{{61,63},5},{{61,67},4},{{61,75},3},{{62,63},6},{{64,65},7},{{64,67},5},{{66,67},7},{{68,69},7},{{68,71},5},{{68,75},4},{{70,71},7},{{72,73},7},{{72,75},6},{{74,75},7},{{76,77},7},{{76,80},5},{{76,88},3},{{76,100},3},{{78,80},6},{{79,80},7},{{81,82},7},{{81,84},6},{{81,88},5},{{83,84},7},{{85,86},8},{{85,88},6},{{87,88},8},{{89,91},6},{{89,94},5},{{89,100},4},{{90,91},8},{{92,94},6},{{93,94},8},{{95,97},6},{{95,100},6},{{96,97},8},{{98,100},6},{{99,100},8}};
void Sort(int l,int r,vector<int> pos,int n,int *P){
	if(l>r) return;
	if(l==r){P[pos[0]]=l;return;}
	int cur=mp[{l,r}],cnt=0;
	memset(A,0,sizeof A);
	for(int x:pos) A[x]=cur;
	Play();
	for(int x:pos) cnt+=(B[x]<=cur);
	vector<int> L,R;
	for(int x:pos) (B[x]<=cur?L:R).push_back(x);
	Sort(l,l+cnt-1,L,n,P),Sort(l+cnt,r,R,n,P);
}
void allValues(int n,int m,int *P){
	static int pos[N];
	For(i,0,n-1) pos[i]=i;
	if(m==200){
		stable_sort(pos,pos+n,[&](int x,int y){
			For(i,0,n-1) A[i]=0;
			A[x]=A[y]=100;
			Play();
			return B[x]<B[y];
		});
		For(i,0,n-1) P[pos[i]]=i+1;
	}else{
		vector<int> Pos(pos,pos+n);
		Sort(1,n,Pos,n,P);
	}
}
