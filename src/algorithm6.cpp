#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

class alg6
{
public:
    vector<int> alg6f(vector<vector<pairs>> &G, int q, int k, double theta);
    vector<int> globalp(int q, int k, vector<vector<pairs>> &G);
    vector<vector<int>> sampleG(vector<vector<pairs>> &G);
};

/**
 * 把概率图视为确定图执行相同的global算法，返回确定图上q的社区
 * */

vector<int> alg6::globalp(int q, int k, vector<vector<pairs>> &G)
{
    int n = G.size();

    // 第一步：不断删除度<k的顶点
    vector<int> deg(n);
    for(int i = 0; i < n; i++) deg[i] = G[i].size();
    vector<bool> inans(n, true);
    while(1){
        vector<int> temp;
        for(int i = 0; i < n; i++){     
            if(inans[i] && deg[i] < k) temp.emplace_back(i);
        }
        if(temp.empty()) break;
        for(auto c : temp){
            inans[c] = false;
            for(auto w : G[c]){ // w = c's neighbor
                deg[w.y]--;
            }
        }
    }
    
    // 第二步：返回包含q的连通分量
    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(inans[i]) ans.emplace_back(i);
    }
    

    UnionFind uf(n);
    for(auto c : ans){
        for(auto w : G[c]){
            if(inans[w.y]) uf.unite(c, w.y);
        }
    }
    
    // 返回结果
    vector<int> res;
    for(auto c : ans){
        if(uf.connected(q, c)) res.emplace_back(c);
    }
        
    return res;
}

vector<vector<int>> alg6::sampleG(vector<vector<pairs>> &G)
{
    int n = G.size();
    vector<vector<int>> ans(n);
    for(int i = 0; i < n; i++){
        for(auto w : G[i]){
            if(i < w.y){ // 避免同一条无向边采样两次
                double prob = rand() / (double)RAND_MAX;
                if(w.prob > prob){
                    ans[i].emplace_back(w.y);
                    ans[w.y].emplace_back(i);
                }
            }
        }
    }
    return ans;
}

vector<int> alg6::alg6f(vector<vector<pairs>> &G, int q, int k, double theta)
{
    int n = G.size();
    vector<int> remain = globalp(q, k, G);
    unordered_map<int,bool> mp;
    for(auto v : remain) mp[v] = true;
    vector<vector<pairs>> H(n);
    for(int i = 0; i < n; i++){
        if(mp[i]){
            for(auto w : G[i]){
                if(mp[w.y]) H[i].emplace_back(w);
            }
        }
    }
    
    printf("Remain size: %d\n", (int)remain.size());

    locals ls;
    int s = 1000;
    vector<int> nums(n);
    for(int i = 0; i < s; i++){
        vector<vector<int>> DH = sampleG(H);
        vector<int> temp = ls.global(q, k, DH);
        for(auto v : temp){
            nums[v]++;
        }
        printf("i= %d/%d\n", i, s);
    }

    vector<int> ans;
    int minn = s * theta;
    for(int i = 0; i < n; i++){
        if(nums[i] >= minn) ans.emplace_back(i);
    }
    return ans;
}

int main(void)
{
    string filename = "biomine.txt";
    base b;
    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);
    printf("V=%d, E=%d\n", b.V, b.E);
    alg6 ag6;
    int q = 484945;
    int k = 23;
    double theta = 0.01;
    START
    vector<int> ans = ag6.alg6f(G, q, k, theta);
    END("ALG 6");
    printf("C size: %d\n", (int)ans.size());

    locals ls;
    auto DG = b.readFile(filename);
    START
    ans = ls.global(q, k, DG);
    END("DETER GLOBAL");
    printf("C size: %d\n", (int)ans.size());
}