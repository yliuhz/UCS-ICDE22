#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/core_decomposition.h"

using namespace std;

/*
** From <<Core decomposition of uncertain graphs>>, SIGKDD 14
** For deterministic graphs (确定图核分解算法)
** Time complexity: O(n+m)
** @params: Graph G
** @return: The core number of each vertex
*/

vector<int> func(vector<vector<int>> &G)
{
    int n = G.size();
    vector<int> c(n), d(n);
    unordered_map<int,vector<int>> D;
    vector<bool> visit(n, false);
    for(int i = 0; i < n; i++){
        d[i] = G[i].size();
        D[d[i]].emplace_back(i);
    }
    for(int k = 0; k < n; k++){
//        printf("%d/%d\n", k+1, n);
        while(!D[k].empty()){
            int v = D[k].back();
            D[k].pop_back();
            if(visit[v]) continue;
            c[v] = k;
            for(auto u : G[v]){
                if(!visit[u] && d[u] > k){
                    D[d[u]-1].emplace_back(u);
                    D[d[u]].erase(find(D[d[u]].begin(), D[d[u]].end(), u));
                    d[u] = d[u] - 1;
                }
            }
            visit[v] = true;
        }
    }
    return c;
}

int main(void)
{
    vector<string> files = {
            "../data/sync_10_0.txt",
            "../data/sync_10_1.txt",
            "../data/sync_10_2.txt",
            "../data/sync_10_3.txt",
            "../data/sync_10_4.txt",
            "../data/sync_20_0.txt",
            "../data/sync_20_1.txt",
            "../data/sync_20_2.txt",
            "../data/sync_20_3.txt",
            "../data/sync_20_4.txt",
            "../data/sync_30_0.txt",
            "../data/sync_30_1.txt",
            "../data/sync_30_2.txt",
            "../data/sync_30_3.txt",
            "../data/sync_30_4.txt",
            "../data/sync_40_0.txt",
            "../data/sync_40_1.txt",
            "../data/sync_40_2.txt",
            "../data/sync_40_3.txt",
            "../data/sync_40_4.txt",
            "../data/sync_50_0.txt",
            "../data/sync_50_1.txt",
            "../data/sync_50_2.txt",
            "../data/sync_50_3.txt",
            "../data/sync_50_4.txt",
    };

    for(auto filename : files){
        base b;
        auto G = b.readFile(filename);
        printf("V=%d, E=%d\n", b.V, b.E);
        // b.printg(G);
        auto c = func(G);
        b.writeToFile(filename+"_core.txt", c);
    }
}