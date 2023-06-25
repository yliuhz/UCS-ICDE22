#include "../include/rss-vl.h"

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

vector<double> rss_kernel(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                          vector<int> &eids, vector<int> &sids,
                          int q, int k, double theta, int N,
                          int tau=100, int start=0)
{
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
        vector<double> temp_2(G.size(), 0.0);

        int k0 = k;
        double p0 = 0.5;
        int r = 50;

        int ridx = 0;
        for(int i = start; i < Edges.size(); i++){
            int u = get<0>(Edges[i]);
            int v = get<1>(Edges[i]);
            double prob = get<2>(Edges[i]);
            if(G[u].size() + G[v].size() > k0 && prob < p0){
                swap(Edges[i], Edges[start+ridx]);
                ridx++;
                if(ridx == r) break; // 至多选择r条边
            }
        }

        if(ridx > 0){
            for(int i = 0; i < ridx+1; i++){
                // basis中的范围:[0,ridx)
                // Omega_i: [0,i-1]选择，[i]丢弃,[i+1,ridx)采样
                double pi = 1.0;
                for(int j = start; j < start+i; j++) {
                    pi *= get<2>(Edges[j]);
                    eids.emplace_back(j);
                }

                if(i < ridx) pi *= 1 - get<2>(Edges[start+i]);

                for(int j = start+i+1; j < start+ridx; j++) sids.emplace_back(j);

                int Ni = ceil(N * pi);

                auto temp = rss_kernel(G, Edges, eids, sids, q, k, theta, Ni, tau, start+ridx);

                // 还原eids和sids
                for(int j = start; j < start+i; j++) eids.pop_back();
                for(int j = start+i+1; j < start+ridx; j++) sids.pop_back();

                for(auto &c : temp) c *= pi;

                for(int l = 0; l < G.size(); l++){
                    temp_2[l] += temp[l];
                }
            }

            return temp_2;
        }
        else{
            // 本轮已无法分层，转到Monte Carlo采样
            auto temp = rss_kernel(G, Edges, eids, sids, q, k, theta, N, N+1, start);
            return temp;
        }
    }
}

vector<int> rss_vl(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                   int q, int k, double theta,
                   vector<double> &Pr_v)
{
    vector<int> eids, sids;

    int N = 500;

    Pr_v = rss_kernel(G, Edges, eids, sids, q, k, theta, N);

    vector<int> ans;
    for(int i = 0; i < G.size(); i++){
        if(Pr_v[i] >= theta) ans.emplace_back(i);
    }
    return ans;
}

int main(void)
{
    string filename = "../data/smallp.txt";
    base b;

    auto G = b.readFilep_from_exist(filename);
    auto Edges = b.readFileE(filename);

    printf("V=%d, E=%d\n", b.V, b.E);

    vector<double> Pr_v;

    int q = 0;
    int k = 2;
    double theta = 0.3;

    START
    auto ans = rss_vl(G, Edges, q, k, theta, Pr_v);
    END("RSS-VL")
}