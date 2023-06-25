//
// Created by liuyue on 17/4/2021.
//

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

/**
 *
 * @param G：剪枝I后的不确定图
 * @param C: 候选集，即待定状态的顶点
 * @param q：查询顶点
 * @param k：
 * @param theta
 * @return
 */

vector<int> ss_vl_v(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  vector<int> &C, int q, int k, double theta,
                  vector<double> &Pr_v)
{
    // Parameters
    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();

    int ridx = 0;
    vector<int> basis(r);
    unordered_set<int> visited;
    for(int i = 0; i < n; i++){
        if(G[i].size() >= k && i != q && visited.find(i) == visited.end()) {
            basis[ridx++] = i;
            for(auto vps : G[i]){
                visited.insert(vps.y);
            }
            visited.insert(i);
        }
        if(ridx == r) break; // 至多选择r个点
    }

    if(ridx == 0){
        cerr<<"BASIS is empty!!!"<<endl;
        return vector<int>{-1};
    }

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < ridx+1; i++){
        // basis中的范围:[0,ridx)
        // Omega_i: [0,i-1]丢弃连接的所有边，[i]选择连接的所有边,[i+1,ridx)采样连接的所有边
        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++){
            int v = basis[j];
            for(auto ups : G[v]){
                pi *= 1 - ups.prob;
            }
        }

        // ASSERT: 中止
    }
}

int main(void)
{
    string filename = "../data/OCnodeslinks.txt.txt";

    base b;
    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);
    printf("V=%d, E=%d\n", b.V, b.E);

    int q = 1168;
    int k = 10;
    double theta = 0.01;

    vector<int> C(b.V);
    iota(C.begin(), C.end(), 0);

    vector<double> Pr_v;

    START
    auto ans = ss_vl_v(G, Edges, C, q, k, theta, Pr_v);
    END("SS-VL")
    printf("size=%d\n", (int)ans.size());
}
