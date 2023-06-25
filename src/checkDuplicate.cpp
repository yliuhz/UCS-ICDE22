//
// Created by liuyue on 31/3/2021.
//

#include <bits/stdc++.h>

using namespace std;

// 删除图数据中的有向重复边，因为我们使用的是无向图

int main(void)
{
    string filename = "../data/dblp-graph-p-18-20.txt";

    unordered_map<int, vector<int>> mp;
    vector<tuple<int,int,double>> Edges;

    ifstream in(filename);
    string temp;
    int x, y; double prob;

    int edge_cnt = 0;

    while(getline(in, temp)){
        edge_cnt++;
        stringstream ss(temp);
        ss>>x; ss>>y; ss>>prob;
        if(find(mp[x].begin(), mp[x].end(), y) == mp[x].end()){
            mp[x].emplace_back(y);
            mp[y].emplace_back(x);
            if(x > y) swap(x, y);
            Edges.emplace_back(x, y, prob);
        }
        else{
            printf("x=%d, y=%d\n", x, y);
        }
    }

    in.close();

    cout<<"原数据边数: "<<edge_cnt<<" , 处理后边数: "<<Edges.size()<<endl;
    cout<<"删除边数: "<<edge_cnt-Edges.size()<<endl;

    int cnt = 0;

    if(Edges.size() < edge_cnt){
        string outfile = filename + ".txt";
        ofstream out(outfile);
        for(auto e : Edges){
            cnt++;
            x = get<0>(e);
            y = get<1>(e);
            prob = get<2>(e);
            out<<x<<" "<<y<<" "<<prob<<endl;
        }
        out.close();
    }

    cout<<"cnt="<<cnt<<endl;
}
