//
// Created by liuyue on 23/3/2021.
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

/**Implement of Algorithm * in my paper skeleton and BSS-II in "Recursive Stratified Sampling:
 * A NewFramework for Query Evaluation on Uncertain Graphs"
 * Random select r edges to stratify the possible world space.
 *
 * @param G: uncertain network
 * @param Edges: the edge list of the uncertain network
 * @param q: query vertex
 * @param k: degree threshold
 * @param theta: probability threshold
 * @param uc_ret: the probability vector, reporting every node's probability
 * @return the (k,theta)-community of q
 */

vector<int> ssampling(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                       int q, int k, double theta, vector<double> &uc_ret, int r=100,
                       double epsilon=0.1, double delta=0.1)
{
    cout<<"//--------ssamping---------//"<<endl;

    int n = G.size();
    int s = nsamples(n, epsilon, delta);
    cout<<"s="<<s<<endl;

    // 分层抽样
    vector<double> uc(n, 0.0);
    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    locals lo;
    for(int i = 0; i <= r; i++){ // 一共r+1个组
        // [0,i-1]的边存在,i的边不存在,[i+1,]的边待采样
        double pi = 1.0;
        for(int j = 0; j < i; j++) pi *= get<2>(Edges[j]);
        if(i < r) pi *= 1 - get<2>(Edges[i]);

        int si = ceil(s * pi); // 第i层的采样个数; 上取整

        for(int l = 0; l < si; l++) {
            vector<int> eids(i);
            iota(eids.begin(), eids.end(), 0);
            for (int j = i+1; j < Edges.size(); j++) {
                // Monte Carlo Sampling
                double prob = urd(re);
                if (get<2>(Edges[j]) >= prob) eids.emplace_back(j);
            }

            if (eids.size() < (k + 1) * k / 2) continue;

            auto H = extractSubgraph(eids, Edges);
            auto Ci = lo.global(q, k, H);

            for (auto v : Ci) {
                uc[v] += pi * 1.0 / si;
            }
        }
    }

    // 返回uc>=theta的顶点
    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(uc[i] >= theta) ans.emplace_back(i);
    }

    for(int i = 0; i < n; i++){
        cout<<"i="<<i<<",uc="<<uc[i]<<endl;
    }

    uc_ret = uc;

    return ans;
}

/**Implement of Algorithm3 in my paper skeleton and BSS-II in "Recursive Stratified Sampling:
 * A NewFramework for Query Evaluation on Uncertain Graphs"
 * Use community's edges to stratify the possible world space
 *
 * @param G: uncertain network
 * @param Edges: the edge list of the uncertain network
 * @param q: query vertex
 * @param k: degree threshold
 * @param theta: probability threshold
 * @param uc_ret: the probability vector, reporting every node's probability
 * @return the (k,theta)-community of q
 */

vector<int> s2sampling(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      int q, int k, double theta, vector<double> &uc_ret,
                      double epsilon=0.1, double delta=0.1)
{
    cout<<"//--------s2samping---------//"<<endl;

    int n = G.size();
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

    // 分层抽样
    vector<double> uc(n, 0.0);
    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());
    for(int i = 0; i <= cnt; i++){ // 一共cnt+1个组
        // [0,i-1]的边存在,i的边不存在,[i+1,]的边待采样
        double pi = 1.0;
        for(int j = 0; j < i; j++) pi *= get<2>(Edges[j]);
        if(i < cnt) pi *= 1 - get<2>(Edges[i]);

        int si = ceil(s * pi); // 第i层的采样个数; 上取整

        for(int l = 0; l < si; l++) {
            vector<int> eids(i);
            iota(eids.begin(), eids.end(), 0);
            for (int j = i+1; j < Edges.size(); j++) {
                // Monte Carlo Sampling
                double prob = urd(re);
                if (get<2>(Edges[j]) >= prob) eids.emplace_back(j);
            }

            if (eids.size() < (k + 1) * k / 2) continue;

            auto H = extractSubgraph(eids, Edges);
            auto Ci = lo.global(q, k, H);

            for (auto v : Ci) {
                uc[v] += pi * 1.0 / si;
            }
        }
    }

    // 返回uc>=theta的顶点
    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(uc[i] >= theta) ans.emplace_back(i);
    }

    for(int i = 0; i < n; i++){
        cout<<"i="<<i<<",uc="<<uc[i]<<endl;
    }

    uc_ret = uc;

    return ans;
}

/** The kernel function of rss_s
 *
 * @param G
 * @param Edges
 * @param eids
 * @param sids
 * @param q
 * @param k
 * @param theta
 * @param N
 * @param uc
 * @param tau
 * @param r
 * @param level
 * @return the probability vector, reporting every node's probability
 */

vector<double> rss(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                vector<int> &eids, vector<int> &sids,
                int q, int k, double theta, int N,
                int tau=100, int r=100, int level=0)
{
//    cout<<"level="<<level<<endl;

    if(N < tau || Edges.size() - level*r < r){
        int n = G.size();
        vector<double> temp(n, 0.0);

        uniform_real_distribution<double> urd(0.0, 1.0);
        random_device dev;
        default_random_engine re(dev());
        locals lo;
        auto eids0 = eids;
        for(int j = 0; j < N; j++){ // N=采样个数
            eids = eids0;
            // 所有待采样的边: sids中的边+[level*r,end)的边
            for(auto idx : sids){
                double prob = urd(re);
                if(get<2>(Edges[idx]) >= prob){
                    eids.emplace_back(idx);
                }
            }
            for(int l = level*r; l < Edges.size(); l++){
                double prob = urd(re);
                if(get<2>(Edges[l]) >= prob){
                    eids.emplace_back(l);
                }
            }

            auto H = extractSubgraph(eids, Edges);
            auto Ci = lo.global(q, k, H);

            for(auto u : Ci){
                temp[u] += 1.0 / N;
            }
        }

        eids = eids0; // 还原eids
        return temp;
    }
    else{
        // 选择前r条边做分层的依据
        vector<double> temp_2(G.size(), 0.0);

        for(int i = 0; i <= r; i++){ // 一共r+1组
            // 0 * * ... * *
            // 1 0 * ... * *
            // 1 1 0 ... * *
            // ...
            // 1 1 1 ... 1 *

            // [0,i)的边存在, i的边不存在, [i+1,r)的边待采样
            double pi = 1.0;
            int sta = level * r; // 本轮起始下标
            for(int j = sta; j < sta+i; j++){
                pi *= get<2>(Edges[j]);
                eids.emplace_back(j);
            }
            if(i < r) pi *= 1 - get<2>(Edges[sta+i]); // 最后一组没有确定不存在的边
            for(int j = sta+i+1; j < sta+r; j++) sids.emplace_back(j); // sids=待采样的下标

            int si = ceil(pi * N);

            auto temp = rss(G, Edges, eids, sids, q, k, theta, si, tau, r, level+1);

            // 还原eids和sids
            for(int j = sta; j < sta+i; j++) eids.pop_back();
            for(int j = sta+i+1; j < sta+r; j++) sids.pop_back();

            for(auto &c : temp) c *= pi;

//            if(level == 0){
//                for(int u = 0; u < G.size(); u++){
//                    uc[u] += temp[u];
//                }
//            }
            for(int l = 0; l < G.size(); l++){
                temp_2[l] += temp[l];
            }
        }

        return temp_2;
    }
}

/** Implement of Algorithm 4 of my paper skeleton and RSS-I in
 * "Recursive Stratified Sampling:  A NewFramework for Query Evaluation on Uncertain Graphs"
 *  Random select r edges to stratify the possible world space.
 *
 * @param G: uncertain graph
 * @param Edges: edge representation of the uncertain graph
 * @param q: query vertex
 * @param k: degree threshold
 * @param theta: probability threshold
 * @param uc_ret: a probability vector, reporting every node's probability
 * @return nodes with probability not less than theta
 */

vector<int> rss_s(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  int q, int k, double theta, vector<double> &uc_ret,
                  double epsilon=0.1, double delta=0.1)
{
    cout<<"//-----------rssampling----------//"<<endl;

    int n = G.size();
    vector<int> eids, sids; // eids: 确定存在的下标; sids: 待采样的下标
    int s = nsamples(n, epsilon, delta);
    cout<<"s="<<s<<endl;

    auto uc = rss(G, Edges, eids, sids, q, k, theta, s); // 执行递归分层抽样

    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(uc[i] >= theta) ans.emplace_back(i);
    }

    for(int i = 0; i < n; i++){
        cout<<"i="<<i<<",uc="<<uc[i]<<endl;
    }

    uc_ret = uc;
    return ans;
}

/** The kernel function of rss2_s
 *
 * @param G
 * @param Edges
 * @param eids
 * @param sids
 * @param q
 * @param k
 * @param theta
 * @param N
 * @param uc
 * @param tau
 * @param r
 * @param level
 * @return the probability vector, reporting every node's probability
 */

vector<double> rss2(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                   vector<int> &eids, vector<int> &sids,
                   int q, int k, double theta, int N,
                   int tau=100, int start=0)
{
//    cout<<"level="<<level<<endl;

    if(N < tau){
        int n = G.size();
        vector<double> temp(n, 0.0);

        uniform_real_distribution<double> urd(0.0, 1.0);
        random_device dev;
        default_random_engine re(dev());
        locals lo;
        auto eids0 = eids;
        for(int j = 0; j < N; j++){ // N=采样个数
            eids = eids0;
            // 所有待采样的边: sids中的边+[level*r,end)的边
            for(auto idx : sids){
                double prob = urd(re);
                if(get<2>(Edges[idx]) >= prob){
                    eids.emplace_back(idx);
                }
            }
            for(int l = start; l < Edges.size(); l++){
                double prob = urd(re);
                if(get<2>(Edges[l]) >= prob){
                    eids.emplace_back(l);
                }
            }

            auto H = extractSubgraph(eids, Edges);
            auto Ci = lo.global(q, k, H);

            for(auto u : Ci){
                temp[u] += 1.0 / N;
            }
        }

        eids = eids0; // 还原eids
        return temp;
    }
    else{
        // 选择前r条边做分层的依据
        vector<double> temp_2(G.size(), 0.0);

        locals lo;
        vector<tuple<int,int,double>> Edges_H;
        Edges_H.assign(Edges.begin()+start, Edges.end());
        auto H = lo.edges2Table(Edges_H);
        auto C = lo.local(q, k, H, H.size(), Edges_H.size());

        if(!C.empty()){
            unordered_set<int> ss(C.begin(), C.end());
            int r = 0;
            for(int i = start; i < Edges.size(); i++){
                int u = get<0>(Edges[i]);
                int v = get<1>(Edges[i]);
                if(ss.count(u) && ss.count(v)){
                    swap(Edges[i], Edges[start+r]); //把G[C]中的边移到[Start,Start+r)位置上
                    r++;
                }
            }

            for(int i = 0; i <= r; i++){ // 一共r+1组
                // 0 * * ... * *
                // 1 0 * ... * *
                // 1 1 0 ... * *
                // ...
                // 1 1 1 ... 1 *

                // [0,i)的边存在, i的边不存在, [i+1,r)的边待采样
                double pi = 1.0;
                for(int j = start; j < start+i; j++){
                    pi *= get<2>(Edges[j]);
                    eids.emplace_back(j);
                }
                if(i < r) pi *= 1 - get<2>(Edges[start+i]);
                for(int j = start+i+1; j < start+r; j++) sids.emplace_back(j); // 待采样下标

                int si = ceil(pi * N);

                auto temp = rss2(G, Edges, eids, sids, q, k, theta, si, tau, start+r);

                // 还原eids和sids
                for(int j = start; j < start+i; j++) eids.pop_back();
                for(int j = start+i+1; j < start+r; j++) sids.pop_back();

                for(auto &c : temp) c *= pi;

                for(int l = 0; l < G.size(); l++){
                    temp_2[l] += temp[l];
                }
            }

            return temp_2;
        }
        else{
            // 本轮已无法分层，转到Monte Carlo采样
            auto temp = rss2(G, Edges, eids, sids, q, k, theta, N, N+1, start);
            return temp;
        }
    }
}

/** Implement of Algorithm * of my paper skeleton
 *  Use community's edges to stratify the possible world space.
 *
 *
 * @param G: uncertain graph
 * @param Edges: edge representation of the uncertain graph
 * @param q: query vertex
 * @param k: degree threshold
 * @param theta: probability threshold
 * @param uc_ret: a probability vector, reporting every node's probability
 * @return nodes with probability not less than theta
 */

vector<int> rss2_s(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  int q, int k, double theta, vector<double> &uc_ret,
                  double epsilon=0.1, double delta=0.1)
{
    cout<<"//-----------rs2sampling----------//"<<endl;

    int n = G.size();
    vector<int> eids, sids; // eids: 确定存在的下标; sids: 待采样的下标
    int s = nsamples(n, epsilon, delta);
    cout<<"s="<<s<<endl;

    auto uc = rss2(G, Edges, eids, sids, q, k, theta, s); // 执行递归分层抽样

    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(uc[i] >= theta) ans.emplace_back(i);
    }

    for(int i = 0; i < n; i++){
        cout<<"i="<<i<<",uc="<<uc[i]<<endl;
    }

    uc_ret = uc;
    return ans;
}

int main(int argc, char *argv[])
{
    string filename = "../data/smallp.txt";

    base b;
    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);

    printf("V=%d,E=%d\n", b.V, b.E);

    int q = 0;
    int k = 2;
    double theta = 1e-10;

    cout<<"q="<<q<<",k="<<k<<",theta="<<theta<<endl;

    vector<double> uc1, uc2, uc3, uc4;

    START
    auto ans = ssampling(G, Edges, q, k, theta, uc1);
    END("ssampling")
    printf("C size=%d\n", (int)ans.size());

    START
    ans = s2sampling(G, Edges, q, k, theta, uc2);
    END("s2sampling")
    printf("C size=%d\n", (int)ans.size());

    START
    ans = rss_s(G, Edges, q, k, theta, uc3);
    END("rs_sampling");
    printf("C size=%d\n", (int)ans.size());

    START
    ans = rss2_s(G, Edges, q, k, theta, uc4);
    END("rs2_sampling");
    printf("C size=%d\n", (int)ans.size());
}
