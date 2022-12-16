#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<map>
#include<queue>
using namespace std;
using ll=long long;

typedef struct drink{
    int id;
    string drink_name;
    int size;
    string time;
}drink;


typedef struct machine{
    bool used; //使用中か否か
    int remain; // 作成可能量
    vector<int> made; // 作成しているドリンクの製造番号
}machine;

void error(){
    cout<<"invalid input"<<endl;
    return;
}

void print_make(string drink_name,int drink_sum,int machine_id){
    cout<<"make: "<<machine_id+1<<" "<<drink_name<<" "<<drink_sum<<endl;
}

void serve_print(vector<int> id){
    cout<<"serve to: ";
    for(int i=0;i<(int)id.size();i++){
        printf("%06d ",id[i]);
    }
    printf("\n");
    return;
}

int main(void){
    ll X,N,M,S,T,K=0; 
    map<string,int> drink_list;
    queue<drink> all;// すべてのドリンクの待ち行列を管理
    /*
        N：ドリンクの種類
        M：機械の数
        S：一度に作成できる最大量
        T：動作にかかる時間
    */
    string line, s;
    cin>>X;
    cin>>N>>M>>S>>T;
    vector<queue<drink>> drinks(N);//N 個のドリンクの待ち行列を管理
    vector<machine> machines(M);  // マシンの管理
    map<int,int> maked; //作成した整理番号を記録
    map<int,int> used_num; //既に使用した整理番号を保持 
    for(int i=0;i<M;i++){
        machines[i].used=false;
        machines[i].remain=S;
    }
    if(X==1){
        cin>>K;
    }
    for(int i=0;i<N;i++){
        cin>>s;
        drink_list[s]=i;
    }
    
    while(getline(cin, line)){ // 入力文字列を取得しlineに格納){
        stringstream line2(line); // stringstream型にする
        vector<string> ss;
        int count = 0;
        while( getline(line2, s, ' ') ) { // stringstream型の入力文字列を半角空白で分割しsに格納
            ss.push_back(s);
            count++;
        }
        if(count==4){
            //cout<<"--- order ---"<<endl;
            // 注文フォーマットと判定
            int id,size;
            string drink_name,time;
            id=atoi(ss[0].c_str());
            drink_name=ss[1];
            if(ss[2]=="S"){
                size=300;
            }else if(ss[2]=="M"){
                size=500;
            }else{
                size=700;
            }
            time=ss[3];
            
            used_num[id]++;
            int flag=0; // 空いているマシーンがあるかを記録
            // マシンの空きを確認
            for(int i=0;i<M;i++){
                int drink_sum=0;
                if(machines[i].used==false){
                    flag=1;
                    drink now;
                    now.id=id;now.drink_name=drink_name;
                    now.size=size;now.time=time;
                    machines[i].used=true;
                    machines[i].remain-=size;
                    machines[i].made.push_back(id);
                    //print_make(now,i);
                    drink_sum+=now.size;
                    while(!drinks[drink_list[drink_name]].empty()){
                        now=drinks[drink_list[drink_name]].front();
                        if(machines[i].remain >= now.size){
                            drinks[drink_list[drink_name]].pop();
                            machines[i].remain-=now.size;
                            machines[i].made.push_back(now.id);
                            //print_make(now,i);
                            drink_sum+=now.size;
                        }else{
                            break;
                        }
                    }
                    print_make(drink_name,drink_sum,i);
                }
            }
            if(flag==0){
                // 待ち行列に注文されたドリンクを追加.
                drink now;
                now.id=id;now.drink_name=drink_name;
                now.size=size;now.time=time;
                all.push(now);// 全体の待ち行列に追加
                drinks[drink_list[drink_name]].push(now); //そのドリンクの待ち行列に追加
            }

        }else if(count==2){
            //cout<<"--- complete ---"<<endl;
            // 完了フォーマットと判定
            int machine_id;
            machine_id = atoi(ss[1].c_str())-1; //機械の番号はss[1]に格納.
            
            //cout<<"machine_id : "<<machine_id<<endl;
            machine now=machines[machine_id];
            machines[machine_id].used=false;
            machines[machine_id].remain=S;


            vector<int> serve_ids;// 提供する飲み物の整理番号の配列
            for(int i=0;i<(int)now.made.size();i++){
                //serve_print(now.made[i]);
                serve_ids.push_back(now.made[i]);
                // 作成したものを作成済みリストに追加.
                maked[now.made[i]]++;
            }
            serve_print(serve_ids);

            machines[machine_id].made.clear();



            // 空いた機械で待ち行列の先頭の物を作る
            int drink_sum=0;
            string make_drink_name;
            while(!all.empty()){
                drink now=all.front();
                // 既に作ってある場合にはスルー
                if(maked.count(now.id)!=0){
                    all.pop();
                    continue;
                }
                // そうでない場合にはその種類のものを作成
                make_drink_name=now.drink_name;// 作成するドリンクの種類を決定.
                all.pop();
                int i=machine_id;
                machines[i].used=true;
                machines[i].remain-=now.size;
                machines[i].made.push_back(now.id);
                drink_sum+=now.size;
                //print_make(now,i);
                maked[now.id]++; //正確には処理完了ではなく, 処理中
                string drink_name=now.drink_name;
                while(!drinks[drink_list[drink_name]].empty()){
                    now=drinks[drink_list[drink_name]].front();
                    if(maked.count(now.id)!=0){
                        // そのドリンクがすでに作成中ならスルー.
                        drinks[drink_list[drink_name]].pop();
                        continue;
                    }
                    if(machines[i].remain >= now.size){
                        drinks[drink_list[drink_name]].pop();
                        machines[i].remain-=now.size;
                        machines[i].made.push_back(now.id);
                        //cout<<"Hello: "<<now.id<<endl;
                        maked[now.id]++;// ここも正確には処理中
                        drink_sum+=now.size;
                        //print_make(now,i);
                    }else{
                        break;
                    }
                }
            }
            print_make(make_drink_name,drink_sum,machine_id);
        }
    }

    return 0;
}