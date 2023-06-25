//
// Created by liuyue on 17/4/2021.
// TIPS: 按照文献中的分层内部分配方式
//

#include "../include/ss-vl.h"

/**
 *
 * @param G：剪枝I后的不确定图
 * @param C: 候选集，即待定状态的顶点
 * @param q：查询顶点
 * @param k：
 * @param theta
 * @return
 */

vector<int> ss_vl_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  vector<int> &C, int q, int k, double theta,
                  vector<double> &Pr_v)
{
    // Parameters
    int N = 500;
    int r = 50;
    int k0 = 2 * k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();

//    vector<tuple<int,int,double>> basis;
//    vector<tuple<int,int,double>> others;
    int ridx = 0;

    for(int i = 0; i < Edges.size(); i++){
        int u = get<0>(Edges[i]);
        int v = get<1>(Edges[i]);
        double prob = get<2>(Edges[i]);
        if(G[u].size() + G[v].size() > k0){
            swap(Edges[ridx++], Edges[i]);
            if(ridx == r) break; // TIPS: 至多选择r条边
        }
    }

//    basis.assign(Edges.begin(), Edges.begin()+ridx);
//    others.assign(Edges.begin()+ridx, Edges.end());

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
        // TIPS: Omega_i: [0,i-1]丢弃，[i]选择,[i+1,ridx)采样
        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++) {
            pi *= 1 - get<2>(Edges[j]);
        }
        if(i < ridx) {
            pi *= get<2>(Edges[i]);
            int u = get<0>(Edges[i]);
            int v = get<1>(Edges[i]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }

        int Ni = ceil(N * pi);

        int startSampling = min(i+1, ridx);

        vector<tuple<int,int,double>> maximum_of_EGi;
        maximum_of_EGi.assign(Edges.begin()+startSampling-1, Edges.end());
        auto maximum_of_Gi = lo.edges2Table(maximum_of_EGi);
        auto Cqk_of_Gi = lo.global(q, k, maximum_of_Gi);
        unordered_set<int> set_of_Cqk_of_Gi(Cqk_of_Gi.begin(), Cqk_of_Gi.end());
        for(int j = startSampling; j < Edges.size(); j++){
            int u = get<0>(Edges[j]), v = get<1>(Edges[j]);
            if(set_of_Cqk_of_Gi.find(u) == set_of_Cqk_of_Gi.end() ||
               set_of_Cqk_of_Gi.find(v) == set_of_Cqk_of_Gi.end()){
                swap(Edges[startSampling++], Edges[j]);
            }
        }

        for(int j = 0; j < Ni; j++){
            vector<vector<int>> Gij = Gi; // 第i层中的第j个采样图

            for(int l = startSampling; l < Edges.size(); l++){ // ASSERT: l==ridx与l==ridx-1采样下标相同
                double random_prob = urd(re);
                double prob = get<2>(Edges[l]);
                if(prob >= random_prob){
                    int u = get<0>(Edges[l]), v = get<1>(Edges[l]);
                    Gij[u].emplace_back(v);
                    Gij[v].emplace_back(u);
                }
            }

            auto Cqk = lo.global(q, k, Gij);
            for(auto u : Cqk){
                Pr[u] += pi * 1.0 / Ni;
                if(Pr[u] - pi * 1.0 / Ni < theta && Pr[u] >= theta){ // TIPS: 保证是第一次达到theta
                    result.emplace_back(u);
                    --remain;
                    if(remain == 0) {
                        for(auto v : C){
                            Pr_v.emplace_back(Pr[v]);
                        }
                        return result;
                    }
                }
            }
        }
    }
//    for(int l = 0; l < n; l++){
//        cout<<l<<": "<<Pr[l]<<endl;
//    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
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
    auto ans = ss_vl_r(G, Edges, C, q, k, theta, Pr_v);
    END("SS-VL-R")
    printf("size=%d\n", (int)ans.size());
}
