//
// Created by liuyue on 30/3/2021.
//

#include <bits/stdc++.h>
#include "../include/locals.h"
#include "../include/base.h"

using namespace std;

int nsamples(int n, double epsilon, double delta)
{
    int s = ceil(1.0/2/epsilon/epsilon*log(2*n/delta));
    return s;
}

vector<vector<int>> extractSubgraph(vector<int> &eids, vector<tuple<int,int,double>> &Edges)
{
    vector<vector<int>> H;
    for(auto idx : eids){
        int x = get<0>(Edges[idx]);
        int y = get<1>(Edges[idx]);
        if((int)H.size()-1 < max(x,y)) H.resize(max(x,y)+1);
        H[x].emplace_back(y);
        H[y].emplace_back(x);
    }
    return H;
}

vector<int> ss_lazy(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      int q, int k, double theta, vector<double> &uc_ret)
{
    cout<<"//--------ss+lazy---------//"<<endl;

    int n = G.size(), m = Edges.size();
    double epsilon = 0.1;
    double delta = 0.1;
    int s = nsamples(n, epsilon, delta);
    cout<<"s="<<s<<endl;

    // 得到对应确定图上的社区，不一定最大，可用global/local
    locals lo;
    auto C = lo.local(q, k, G, n, Edges.size());
    unordered_set<int> ss(C.begin(), C.end());
    int cnt = 0; // 把G[C]中的所有边移到Edges的[0,cnt)位置上
    for(int i = 0; i < Edges.size(); i++){
        int u = get<0>(Edges[i]);
        int v = get<1>(Edges[i]);
        if(ss.count(u) && ss.count(v)){
            swap(Edges[i], Edges[cnt++]);
        }
    }

    // 为每条边声明几何分布变量
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    auto BatchSampling = [](int m, int start, auto &geo_vs, auto &re){
        vector<vector<int>> subg;
        for(int i = start; i < m; i++){
            int idx_of_sample = geo_vs[i](re); // 对第i条边采样一个几何分布数字
            if((int)subg.size()-1 < idx_of_sample) subg.resize(idx_of_sample+1);
            subg[idx_of_sample].template emplace_back(i);
        }
        return subg;
    };

    // 分层抽样
    vector<double> uc(n, 0.0);
    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());
    for(int i = 0; i <= cnt; i++){ // 一共cnt+1个组
        // [0,i-2]的边存在,i-1的边不存在,[i,]的边待采样
        double pi = 1.0;
        for(int j = 0; j < i; j++) pi *= get<2>(Edges[j]);
        if(i < cnt) pi *= 1 - get<2>(Edges[i]);

        int si = ceil(s * pi); // 第i层的采样个数; 上取整

        int hasSampled = 0;

        while(hasSampled < si) {
            auto subg = BatchSampling(m, i + 1, geo_vs, re);

            for (int j = 0; j < subg.size(); j++) {
                vector<int> eids(i);
                iota(eids.begin(), eids.end(), 0);

//                for (auto idx : subg[j]) {
//                    eids.emplace_back(idx);
//                }

                eids.insert(eids.end(), subg[j].begin(), subg[j].end());

                if (eids.size() < (k + 1) * k / 2) continue; // k-core至少(k+1)*k/2条边, 此时为(k+1)-clique

                auto H = extractSubgraph(eids, Edges);
                auto Ci = lo.global(q, k, H);

                for (auto v : Ci) {
                    uc[v] += pi * 1.0 / si;
                }

                if (hasSampled + j + 1 >= si) break;
                // 为subg[j]中的边重新采样几何分布
                int offset = 1;
                for (auto edge_idx : subg[j]) {
                    double idx_of_sample = geo_vs[edge_idx](re) + j + offset;
                    if ((int) subg.size() - 1 < idx_of_sample) subg.resize(idx_of_sample + 1);
                    subg[idx_of_sample].emplace_back(edge_idx);
                }
            }
            hasSampled += subg.size();
        }
    }

    // 返回uc>=theta的顶点
    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(uc[i] >= theta) ans.emplace_back(i);
    }

//    for(int i = 0; i < n; i++){
//        cout<<"i="<<i<<",uc="<<uc[i]<<endl;
//    }

    uc_ret = uc;

    return ans;
}

int main(int argc, char* argv[])
{
    string filename = "../data/dblp-graph-p.txt";
    base b;
    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);

    printf("V=%d, E=%d\n", b.V, b.E);

    int q = 20634;
    int k = 44;
    double theta = 1e-10;

    cout<<"q="<<q<<",k="<<k<<",theta="<<theta<<endl;

    locals lo;
    auto C = lo.global(q, k, G);
    auto H = lo.extractSubgraph(C, G);

    H = lo.rearrange(H, q);
    auto Edges_H = lo.table2Edges(H);

    cout<<"Max C size="<<C.size()<<",H size="<<H.size()<<",Edges_H size="<<Edges_H.size()<<endl;

    vector<double> uc1, uc2;

    START
    auto ans = ss_lazy(H, Edges_H, q, k, theta, uc1);
    END("ss+lazy")
    printf("C size=%d\n", (int)ans.size());

}
