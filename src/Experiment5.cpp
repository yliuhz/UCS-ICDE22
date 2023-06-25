//
// Created by liuyue on 5/4/2021.
// TIPS: 测索引构建需要的时间和内存

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

/**
 *
 * @param G
 * @return the core decomposition of G for every k(k0<=k<=kmax)
 * TIPS: 返回结果取名decomposition, 三维vector, 第一维是k, 第二维是k对应分解中一个连通分量的下标
 */

vector<vector<vector<int>>> core_domposition_every_k(vector<vector<int>> &G)
{
    int k0 = 2, kmax = 2;

    // 为每个顶点计算core_number, 结果保存在向量core_number_cache中
    int n = G.size();
    vector<int> c(n);
    vector<int> d(n);
    unordered_map<int,vector<int>> D;
    vector<bool> visit(n, false);
    for(int i = 0; i < n; i++){
        d[i] = G[i].size();
        D[d[i]].emplace_back(i);
    }

    unordered_map<int,vector<int>> core_number_cache; // TIPS: k2vertices
    for(int k = 0; k < n; k++){
//        printf("%d/%d\n", k+1, n);
//        if(!D[k].empty()) kmax = k;

        while(!D[k].empty()){
            int v = D[k].back();
            D[k].pop_back();
            if(visit[v]) continue;
            c[v] = k;
            kmax = k;
            core_number_cache[k].emplace_back(v);
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

    // 构造并查集, 用于计算所有连通分量
    UnionFind uf(n);
    for(int i = 0; i < n; i++){
        for(auto j : G[i]) uf.unite(i, j);
    }

    cout<<"kmax="<<kmax<<endl;
    // 从小到大遍历k, 中间不断从并查集排除core_number<k的顶点.对每个k输出所有连通分量
    vector<vector<vector<int>>> ret(kmax+1);

    unordered_set<int> cc_set; // TIPS: 存储受影响的连通分量中的顶点，删除后要重建这些顶点的连通分量

    for(int k = 0; k < k0; k++) {

        for(auto v : core_number_cache[k]){
            auto cc = uf.drop(v);

            cc_set.insert(cc.begin(), cc.end());
            cc_set.erase(v);
        }
    }

    for(auto v : cc_set){
        for(auto u : G[v]){
            if(cc_set.find(u) != cc_set.end()) uf.unite(u,v);
        }
    }

    for(int k = k0; k <= kmax; k++){
        auto connected_components = uf.connected_components(k);
        ret[k] = connected_components; // ASSERT: 把连通分量的结果赋给ret[k]

        if(k == kmax) break;

        cc_set.clear();

        for(auto v : core_number_cache[k]){
            auto cc = uf.drop(v);

            cc_set.insert(cc.begin(), cc.end());
            cc_set.erase(v);
        }

        //rebuild
        for(auto v : cc_set){
            for(auto u : G[v]){
                if(cc_set.find(u) != cc_set.end()) uf.unite(u, v);
            }
        }
    }

    return ret;
}

/**
 * TIPS: without proposed union-find structure
 * @param G
 * @return
 */

vector<vector<vector<int>>> core_domposition_every_k_(vector<vector<int>> &G)
{
    int k0 = 2, kmax = 2;

    // 为每个顶点计算core_number, 结果保存在向量core_number_cache中
    int n = G.size();
    vector<int> c(n);
    vector<int> d(n);
    unordered_map<int,vector<int>> D;
    vector<bool> visit(n, false);
    for(int i = 0; i < n; i++){
        d[i] = G[i].size();
        D[d[i]].emplace_back(i);
    }

    unordered_map<int,vector<int>> core_number_cache; // TIPS: k2vertices
    for(int k = 0; k < n; k++){
//        printf("%d/%d\n", k+1, n);
//        if(!D[k].empty()) kmax = k;

        while(!D[k].empty()){
            int v = D[k].back();
            D[k].pop_back();
            if(visit[v]) continue;
            c[v] = k;
            kmax = k;
            core_number_cache[k].emplace_back(v);
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

    // 构造并查集, 用于计算所有连通分量
    UnionFind_ uf(n);
    for(int i = 0; i < n; i++){
        for(auto j : G[i]) uf.unite(i, j);
    }

    cout<<"kmax="<<kmax<<endl;
    // 从小到大遍历k, 中间不断从并查集排除core_number<k的顶点.对每个k输出所有连通分量
    vector<vector<vector<int>>> ret(kmax+1);

    unordered_set<int> cc_set; // TIPS: 存储要删除的顶点

    for(int k = 0; k < k0; k++) {
//        for(auto v : core_number_cache[k]){
//            cc_set.insert(v);
//        }
        cc_set.insert(core_number_cache[k].begin(), core_number_cache[k].end());
    }

//    for(auto v : cc_set){
//        for(auto u : G[v]){
//            if(cc_set.find(u) != cc_set.end()) uf.unite(u,v);
//        }
//    }

    uf.drop_();

    for(int i = 0; i < n; i++){
        if(cc_set.find(i) != cc_set.end()) continue;
        for(auto j : G[i]){
            if(cc_set.find(j) != cc_set.end()) continue;
            uf.unite(i, j);
        }
    }

    for(int k = k0; k <= kmax; k++){
        printf("k=%d/%d\n", k, kmax);

        auto connected_components = uf.connected_components_(k);
        ret[k] = connected_components; // ASSERT: 把连通分量的结果赋给ret[k]

        if(k == kmax) break;

//        for(auto v : core_number_cache[k]){
//            cc_set.insert(v);
//        }

        cc_set.insert(core_number_cache[k].begin(), core_number_cache[k].end());

        uf.drop_();

        for(int i = 0; i < n; i++){
            if(cc_set.find(i) != cc_set.end()) continue;
            for(auto j : G[i]){
                if(cc_set.find(j) != cc_set.end()) continue;
                uf.unite(i, j);
            }
        }

    }

    return ret;
}

/**
 *
 * @param decomposition
 * @param n
 * @return the hashmap of the decomposition
 * TIPS: 返回结果取名hashmap, 二维vector, 第一维是k, 第二维是顶点, 里边的值是该顶点在k-core分解中所在连通分量的下标
 */

vector<vector<int>> build_hashmap(vector<vector<vector<int>>> &decomposition, int n)
{
    int kmax = decomposition.size();
    vector<vector<int>> ret(kmax, vector<int>(n));
    for(int k = 0; k < kmax; k++){
        int idx = -1;
        for(auto cc : decomposition[k]){ // cc = connected components
            ++idx; // 连通分量的下标
            for(auto v : cc){
                ret[k][v] = idx;
            }
        }
    }
    return ret;
}

vector<vector<pairs>> fetch_subgraph(vector<vector<vector<int>>> &decomposition,
                                     vector<vector<int>> &hashmap, int q, int k,
                                     vector<vector<pairs>> &G)
{
    auto idx = hashmap[k][q];
    printf("idx=%d\n", idx);
    printf("size=%d\n", (int)decomposition[k].size());
    auto VH = decomposition[k][idx];
    locals lo;
    auto H = lo.extractSubgraph(VH, G);
    return H;
}

int main(int argc, char *argv[])
{
    vector<string> files = {
            "email-Enron-p.txt",
//            "flickr.txt",
//            "dblp-graph-p-18-20.txt",
//            "biomine.txt",
    };

    vector<double> buildtime(files.size());
    base b;

    clock_t start, end;
    double time;

    string outfile = "output/Experiment5(time).txt";
    ofstream out(outfile);

    for(int i = 0; i < files.size(); i++){
        string file = files[i];
        string filename = "data/" + file;

        printf("Filename: %s\n", file.c_str());
        printf("%s: Reading graph ...\n", file.c_str());

        auto dG = b.readFile(filename);

        for(int j = 0; j < 50; j++){
            start = clock();
            auto decomposition = core_domposition_every_k_(dG);
            auto hashmap = build_hashmap(decomposition, b.V);
            end = clock();
            time = (double)(end - start)/CLOCKS_PER_SEC;

            out<<file<<": ";
            out<<fixed<<setprecision(3)<<time<<" "<<endl;
        }
    }
}
