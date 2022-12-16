#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;
using ll=long long;
static const int MAX = 200005;
long long INFTY = 1000000000000000;
static const int WHITE = 0;
static const int GRAY = 1;
static const int BLACK = 2;


ll n,m,x,y;
vector<pair<ll,pair<ll,ll>>> adj[MAX];//重み付きグラフの有向グラフ隣接リスト表現

void dijkstra(){
    priority_queue<pair<ll,int>> PQ;
    int color[MAX];
    ll d[MAX];
    for(int i=0;i<n;i++){//初期化
        d[i]=INFTY;
        color[i]=WHITE;
    }

    d[x]=0;
    PQ.push(make_pair(0,x));
    //pair<その頂点までの最短距離 , 頂点番号 >
    color[x]=GRAY;

    while(!PQ.empty()){
        pair<long long,long long> f=PQ.top();PQ.pop();
        int u=f.second;
        color[u]=BLACK;

        if(d[u]<f.first*(-1)) continue;

        for(int j=0;j<(int)adj[u].size();j++){
            ll v=adj[u][j].first;
            // vは次の頂点
            if(color[v]==BLACK) continue;
            ll k=adj[u][j].second.second;//k
            ll num;
            if(d[u]%k==0){
                num=d[u];
            }else{
                num=(d[u]-d[u]%k)+k;
            }
            ll t=adj[u][j].second.first+num;
            //cout<<"t:"<<t<<endl;
            if(d[v]>t){//距離の更新が可能ならば
                d[v]=t;
                //priority_queueはデフォルトで大きい値を優先するため -1をかける.
                PQ.push(make_pair(d[v]*(-1),v));
                //cout<<"in:"<<v<<endl;
                color[v]=GRAY;
            }
        }
    }

    if(d[y]==INFTY){
        cout<<-1<<endl;
    }else{
        cout<<d[y]<<endl;
    }
}

int main(){
    ll u,v,c,k;

    cin>>n>>m>>x>>y;
    x--;y--;
    vector<ll> vec(n);
    for(int i=0;i<m;i++){
        cin>>u>>v>>c>>k;
        u--;v--;
        // uからvへ, 重みcの辺をつくる.　
        adj[u].push_back(make_pair(v,make_pair(c,k)));
        adj[v].push_back(make_pair(u,make_pair(c,k)));
    }
    dijkstra();

    return 0;
}