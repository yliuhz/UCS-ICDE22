//
// Created by liuyue on 25/3/2021.
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

/** Implement of algorithm 5 of my paper skeleton
 *
 * @param G: uncertain graph
 * @param Edges: edge representation of the uncertain graph
 * @param q: a query vertex
 * @param k: degree parameter
 * @param theta: probability parameter
 * @return the community of q with nodes' probability >= theta
 */
vector<int> lazySampling(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                         int q, int k, double theta)
{
    int n = G.size(), m = Edges.size();
    double epsilon = 0.1;
    double delta = 0.1;
    int s = nsamples(n, epsilon, delta);
    cout<<"s="<<s<<endl;

    int cnt = 0; // 统计已经采样的个数

    auto BatchSampling = [](int m, auto &geo_vs, auto &re){
        vector<vector<int>> subg;
        for(int i = 0; i < m; i++){
            int idx_of_sample = geo_vs[i](re); // 对第i条边采样一个几何分布数字
            if((int)subg.size()-1 < idx_of_sample) subg.resize(idx_of_sample+1);
            subg[idx_of_sample].template emplace_back(i);
        }
        return subg;
    };

    random_device dev;
    default_random_engine re(dev());
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    vector<int> uc(n, 0);
    locals lo;

    while(cnt < s){
        auto subg = BatchSampling(m, geo_vs, re);
        for(int i = 0; i < subg.size(); i++){ // subg[i]表示第i个采样的确定图中所有边的下标
            auto H = extractSubgraph(subg[i], Edges);
            auto Ci = lo.global(q, k, H);

            for(auto u : Ci){
                uc[u]++;
            }

            // 要对本轮子图中的所有边重新采样
            int offset = 1; // 从下一个样例开始。由于几何分布可能采到0，所以这里加1表示从下一个样例开始
            for(auto edge_idx : subg[i]){
                int idx_of_sample = geo_vs[edge_idx](re) + i + offset;
                if((int)subg.size() - 1 < idx_of_sample) subg.resize(idx_of_sample+1);
                subg[idx_of_sample].emplace_back(edge_idx);
            }

            if(cnt + i + 1 >= s) break; // 一共s个采样
        }
        cnt += subg.size();
    }

    vector<int> ans;
    for(int i = 0; i < n; i++){
        double temp = uc[i] * 1.0 / s;
        if(temp >= theta){
            ans.emplace_back(i);
        }
    }

//    for(int i = 0; i < n; i++){
//        cout<<"i="<<i<<",uc="<<uc[i]*1.0/s<<endl;
//    }

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

    START
    auto ans = lazySampling(H, Edges_H, q, k, theta);
    END("lazySampling")
    printf("C size=%d\n", (int)ans.size());
}
