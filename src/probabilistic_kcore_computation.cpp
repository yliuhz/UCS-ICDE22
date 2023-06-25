//
// Created by liuyue on 2021/3/2.
//

/**
 * <<Efficient probabilistic k-core computation on uncertain graphs>>
 */

#include "../include/probabilistic_kcore_computation.h"

int pkcore::samplesNeeded(double epsilon, double delta) {
    return ceil(1.0/2/epsilon/epsilon*log(2*V/delta));
}

vector<vector<int>> pkcore::sample(vector<tuple<int, int, double>> &Edges, default_random_engine &re) {
    vector<vector<int>> ans(V);
    uniform_real_distribution<double> u(0.0, 1.0);
    for(auto e : Edges){
        double prob = u(re);
//        double prob = rand() / (double)RAND_MAX;
        double e_prob = get<2>(e);
        if(e_prob > prob){
            int x = get<0>(e), y = get<1>(e);
            ans[x].emplace_back(y);
            ans[y].emplace_back(x);
        }
    }
    return ans;
}

vector<vector<int>> pkcore::sample(vector<vector<pairs>> &G, default_random_engine &re) {
    int n = G.size();
    vector<vector<int>> ans(n);
    uniform_real_distribution<double> urd(0.0, 1.0);
    for(int i = 0; i < n; i++){
        for(auto w : G[i]){
            if(i < w.y){ // 避免同一条无向边采样两次
                double prob = urd(re);
                if(w.prob > prob){
                    ans[i].emplace_back(w.y);
                    ans[w.y].emplace_back(i);
                }
            }
        }
    }
    return ans;
}

double pkcore::avg(vector<int> &vs) {
    assert(!vs.empty());
    int n = vs.size();
    int sums = 0;
    for(auto c : vs) sums+=c;
    return sums*1.0/n;
}

vector<int> pkcore::basicSampling(vector<tuple<int,int,double>> &Edges, int k, double theta, double epsilon, double delta) {
    vector<int> nums(V, 0);
    int s = samplesNeeded(epsilon, delta);
    random_device dev;
    default_random_engine re(dev());
    for(int i = 0; i < s; i++){
        auto H = sample(Edges, re);
        vector<int> vs = func(H);
        assert(V == (int)vs.size());
        printf("i=%d, s=%d, max: %d, avg: %.2f\n", i, s, *max_element(vs.begin(), vs.end()), avg(vs));
        for(int j = 0; j < V; j++){
            if(vs[j] >= k) nums[j]++;
        }
    }

    int minn = s * theta;
    vector<int> ans;
    for(int i = 0; i < V; i++){
        if(nums[i] >= minn) ans.emplace_back(i);
    }
    return ans;
}


// @return: P(deg(v)=i)
double pkcore::degEqualK(vector<vector<pairs>> &G, int v, int i) {
    int dv = G[v].size();
    assert(dv>0);
    vector<vector<double>> X(dv, vector<double>(dv+1, 0)); // e_0--e_dv-1
    // deg: 0--dv
    X[0][0] = 1 - G[v][0].prob;
    for(int h = 1; h < dv; h++){
        X[h][0] = X[h-1][0] * (1 - G[v][h].prob);
    }
    X[0][1] = G[v][0].prob;
    for(int h = 1; h < dv; h++){
        for(int j = 1; j <= h+1; j++){
            X[h][j] = X[h-1][j-1] * G[v][h].prob + X[h-1][j] * (1 - G[v][h].prob);
        }
    }
    return X[dv-1][i];
}

// @return: P(deg(v)>=i)
double pkcore::degGeK(vector<vector<pairs>> &G, int v, int i){
    int dv = G[v].size();
//    assert(dv>0);
    if(dv == 0) return 0.0;
    vector<vector<double>> X(dv, vector<double>(dv+1, 0)); // e_0--e_dv-1
    // deg: 0--dv
    X[0][0] = 1 - G[v][0].prob;
    for(int h = 1; h < dv; h++){
        X[h][0] = X[h-1][0] * (1 - G[v][h].prob);
    }
    X[0][1] = G[v][0].prob;
    for(int h = 1; h < dv; h++){
        for(int j = 1; j <= h+1; j++){
            X[h][j] = X[h-1][j-1] * G[v][h].prob + X[h-1][j] * (1 - G[v][h].prob);
        }
    }

    double ans = 0.0;
    for(int j = 0; j < i; j++) ans += X[dv-1][j];
    return 1.0 - ans;
}

vector<double> pkcore::upperb(vector<vector<pairs>> &G, int k, double delta) {
    int n = G.size();
    vector<double> ans(n);
    vector<bool> needUpdate(n, true);
    for(int i = 0; i < n; i++){
        ans[i] = degGeK(G, i, k);
    }

    while(1){
        int uf = -1;
        for(int i = 0; i < n; i++){
            if(needUpdate[i]){
                uf = i; break;
            }
        }
        if(uf == -1) break;
        needUpdate[uf] = false;
        double temp = 0.0;
        for(auto w : G[uf]){
            temp += min(w.prob, ans[w.y]);
        }
        temp /= k;
        if(ans[uf] - temp > delta){
            ans[uf] = temp;
            for(auto w : G[uf]){
                needUpdate[w.y] = true;
            }
        }
    }

    return ans;
}

void pkcore::kcoreMembershipCheck(vector<vector<int>> &G, vector<int> &T, vector<int> &state,
                                         vector<vector<pairs>> &UG, int k) {
    int n = G.size();

    queue<int> Q;
    vector<bool> inqueue(n, false);
    for(auto t : T) {
        Q.push(t); inqueue[t] = true;
    }
    vector<bool> sampled(n, false);

    vector<int> low(n, 0), up(n);
    for(auto t : T) up[t] = UG[t].size();

    auto Increase = [&low, k, &state, &inqueue, &Q](int v){
        low[v] = low[v] + 1;
        if(low[v] >= k){
            state[v] = KEEP;
            if(!inqueue[v]){
                Q.push(v); inqueue[v] = true;
            }
        }
    };

    auto Decrease = [&up, k, &state, &inqueue, &Q](int v){
        up[v] = up[v] - 1;
        if(up[v] < k){
            state[v] = DEL;
            if(!inqueue[v]){
                Q.push(v); inqueue[v] = true;
            }
        }
    };

    while(!Q.empty()){
        int u = Q.front();
        Q.pop();
        inqueue[u] = false;
        sampled[u] = true;

        for(auto v : G[u]){
            bool chosen = false; // choose e(u,v)
            if(state[v] != DEL){
                default_random_engine re;
                uniform_real_distribution<double> urd(0.0, 1.0);
                double prob = urd(re);
                double e_prob;
                for(auto e : UG[u]){
                    if(e.y == v){
                        e_prob = e.prob;
                        break;
                    }
                }
                if(e_prob > prob){
                    chosen = true;
                }
            }

            switch(state[u]){
                case KEEP:{
                    if(state[v] == UNCERTAIN){
                        if(!chosen){
                            Decrease(v);
                        }
                        else{
                            Increase(v);
                        }
                    }
                    break;
                }
                case DEL:{
                    if(state[v] == UNCERTAIN){
                        Decrease(v);
                    }
                    break;
                }
                case UNCERTAIN:{
                    if(!chosen){
                        up[u]--;
                        G[u].erase(find(G[u].begin(), G[u].end(), v));
                        G[v].erase(find(G[v].begin(), G[v].end(), u));
                        if(state[v] == UNCERTAIN){
                            Decrease(v);
                        }
                    }
                    else{
                        if(state[v] == KEEP){
                            low[u]++;
                        }
                        else if(state[v] == DEL){
                            up[u]--;
                        }
                    }

                    if(state[v] != UNCERTAIN){
                        G[u].erase(find(G[u].begin(), G[u].end(), v));
                        G[v].erase(find(G[v].begin(), G[v].end(), u));
                    }

                    if(up[u] < k){
                        state[u] = DEL; Q.push(u); inqueue[u] = true;
                    }
                    else if(low[u] >= k){
                        state[u] = KEEP; Q.push(u); inqueue[u] = true;
                    }

                    break;
                }
            }
        }

        if(state[u] == UNCERTAIN){
            for(auto v : G[u]){
                if(!sampled[v] && state[v] == UNCERTAIN){
                    Q.push(v); inqueue[v] = true;
                }
            }
        }
        else{
            for(auto v : G[u]){
                G[v].erase(find(G[v].begin(), G[v].end(), u));
            }
            G[u].clear();
        }

        bool finished = true;
        for(auto t : T){
            if(state[t] == UNCERTAIN){
                finished = false; break;
            }
        }
        if(finished) return;
    }

    for(auto t : T){
        if(state[t] == UNCERTAIN){
            state[t] = KEEP;
        }
    }
}

vector<vector<int>> pkcore::toDeter(vector<vector<pairs>> &G) {
    int n = G.size();
    vector<vector<int>> ans(n);
    for(int i = 0; i < n; i++){
        for(auto w : G[i]){
            ans[i].emplace_back(w.y);
        }
    }
    return ans;
}

vector<vector<pairs>> pkcore::extractSubgraph(vector<vector<pairs>> &G, vector<int> &vs) {
    int n = G.size();
    unordered_map<int,bool> mp;
    for(auto v : vs) mp[v] = true;
    vector<vector<pairs>> ans(n);
    for(auto u : vs){
        for(auto v : G[u]){
            if(mp[v.y]){
                ans[u].emplace_back(v);
                ans[v.y].emplace_back(pairs{u, v.prob});
            }
        }
    }
    return ans;
}

vector<int> pkcore::advancedSampling(vector<tuple<int, int, double>> &Edges, vector<vector<pairs>> &G, int k,
                                     double theta, double epsilon, double delta) {
    int n = G.size();
    vector<int> state(n, UNCERTAIN);
    vector<vector<int>> dG = toDeter(G);
    vector<int> vs = func(dG);
    for(int i = 0; i < n; i++){
        if(vs[i] < k) state[i] = DEL;
    }

    auto H = extractSubgraph(G, vs);
    vector<double> ub = upperb(H, k, delta);
    for(int i = 0; i < n; i++){
        if(ub[i] < theta) state[i] = DEL;
    }

    vector<int> T, C;
    for(int i = 0; i < n; i++){
        if(state[i] == UNCERTAIN) T.emplace_back(i);
    }

    int s = samplesNeeded(epsilon, delta);
    vector<double> lb(n, 0.0);

    random_device dev;
    default_random_engine re(dev());

    for(int i = 0; i < s; i++){
        auto dH = sample(H, re);
        kcoreMembershipCheck(dH, T, state, H, k);
        for(auto t : T){
            if(state[t] == KEEP){
                lb[t] += 1.0 / s;
            }
            else{
                ub[t] -= 1.0 / s;
            }

            if(lb[t] >= theta){
                T.erase(find(T.begin(), T.end(), t));
                C.emplace_back(t);
            }
            else if(ub[t] < theta){
                T.erase(find(T.begin(), T.end(), t));
            }
        }

        if(T.empty()) return C;
    }
    return C;
}

int main(void)
{
    string filename = "OCnodeslinks.txt.txt";

    base b;
    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);

    printf("V= %d, E= %d\n", b.V, b.E);

    pkcore pkc(b.V, b.E);

    int k = 20;
    double theta = 0.01;
    double epsilon = 0.1;
    double delta = 0.1;

    auto vs = pkc.basicSampling(Edges, k, theta, epsilon, delta);

    printf("vs size: %d\n", (int)vs.size());
}

