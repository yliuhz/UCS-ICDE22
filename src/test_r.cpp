#include <bits/stdc++.h>
#include <unistd.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

map<int,int> mp;
int cnt = 0;
int zero = 0;


// TODO: 社区数量过大
void f(int q, int k, vector<vector<int>> &G, vector<vector<int>> &ans, vector<int> &C, int last = INT_MIN)
{
    // initially C = V
    int V = C.size();
    int E = 0;
    for(auto w : C){
        E += G[w].size();
    }
    E /= 2;


    for(auto v : C){
        int i = v;
        if(i == q || G[i].empty() || i <= last || G[q].size() < k) continue;
//        printf("v= %d, C size= %d\n", v, (int)C.size());

//        if(k > (1+sqrt(9+8*(E-V-G[i].size()+1)))/2) continue; // 过松

        mp[v]++;
        vector<int> temp = G[i];

        for(auto w : G[i]){
            G[w].erase(find(G[w].begin(), G[w].end(), i));
        }
        G[i].clear();

        locals ls;
        vector<int> ret = ls.global(q, k, G);
        sort(ret.begin(), ret.end());

//        if(find(ans.begin(), ans.end(), ret) != ans.end()){
//            printf("cnt= %d, ans size= %d\n", ++cnt, (int)ans.size());
//        }
//
//        if(ret.empty()){
//            printf("zero= %d, ans size= %d\n", ++zero, (int)ans.size());
//        }

        if(!ret.empty() && find(ans.begin(), ans.end(), ret) == ans.end()){
            ans.emplace_back(ret);
            vector<vector<int>> H = ls.extractSubgraph(ret, G);
            auto tp = ls.global(q, k, H);
            printf("ans size= %d, tp size= %d, tp==ret: %d\n", (int)ans.size(), (int)tp.size(), (int)(tp==ret));
            f(q, k, H, ans, ret, last=i);
        }

        G[i] = temp; // rebuild G;
        for(auto w : G[i]){
            G[w].emplace_back(i);
        }
    }
}

void g(int q, int k, vector<vector<int>> &G, vector<vector<int>> &ans, vector<int> &C, int last = INT_MIN)
{

}

int main(void)
{
    string filename = "4clique.txt";
    base b;
    auto G = b.readFile(filename);
    printf("V= %d, E= %d\n", b.V, b.E);

    int maxn = 0;
    for(auto vs : G){
        maxn = max(maxn, (int)vs.size());
    }
    printf("Max deg= %d\n", maxn);

    int q = 289;
    int k = 20;

    vector<vector<int>> ans;
    vector<int> temp;

    locals ls;
    vector<int> C = ls.global(q, k, G);
    G = ls.extractSubgraph(C, G);

    printf("C size= %d\n", (int)C.size());

    START
    f(q, k, G, ans, C);
    END("f");

    printf("ans size= %d\n", (int)ans.size());

    set<vector<int>> S(ans.begin(), ans.end());
    ans.assign(S.begin(), S.end());
    ans.emplace_back(C);

    printf("ans size= %d\n", (int)ans.size());
    for(auto vs : ans){
        for(auto v : vs){
            printf("%d ", v);
        }
        printf("\n");
    }

    for(auto [v, cnt] : mp){
        printf("%d: %d\n", v, cnt);
    }

    printf("cnt= %d\n", cnt);

    return 0;
}