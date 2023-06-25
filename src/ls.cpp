//
// Created by liuyue on 17/4/2021.
//

#include "../include/ls.h"

vector<int> ls(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                       vector<int> &C, int q, int k, double theta,
                       vector<double> &Pr_v, int N)
{
    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = theta;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    random_device dev;
    default_random_engine re(dev());
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
    for(int i = 0; i < m; i++){
        int idx_of_sample = geo_vs[i](re);
        if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
        exist_idx[idx_of_sample].template emplace_back(i);
    }

    for(int i = 0; i < N; i++){
        vector<int> del_list;

        for(auto v : C){
            double remain_prob = (N - i) * 1.0 / N;
            if(Pr[v] + remain_prob < theta){
                --remain;
                del_list.emplace_back(v);
//                set_of_C.erase(v);
                if(remain == 0) {
                    for(auto u : C){
                        Pr_v.emplace_back(Pr[u]);
                    }
                    return result;
                }
            }
        }

        for(auto v : del_list) set_of_C.erase(v);

        vector<vector<int>> Gi(n);
        for(auto idx : exist_idx[i]){
            int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
                --remain;
                set_of_C.erase(u);
                if(remain == 0) {
                    for(auto v : C){
                        Pr_v.emplace_back(Pr[v]);
                    }
                    return result;
                }
            }
        }

        for(auto idx : exist_idx[i]){
            int idx_of_sample = geo_vs[idx](re)+i+1; // 采样数字可能是0，加1保证从下个采样图开始计算
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(idx);
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> ls_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
               vector<int> &C, int q, int k, double theta,
               vector<double> &Pr_v, int N)
{
    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = theta;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    random_device dev;
    default_random_engine re(dev());
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
    for(int i = 0; i < m; i++){
        int idx_of_sample = geo_vs[i](re);
        if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
        exist_idx[idx_of_sample].template emplace_back(i);
    }

    for(int i = 0; i < N; i++){
//        vector<int> del_list;
//
//        for(auto v : C){
//            double remain_prob = (N - i) * 1.0 / N;
//            if(Pr[v] + remain_prob < theta){
//                --remain;
//                del_list.emplace_back(v);
////                set_of_C.erase(v);
//                if(remain == 0) {
//                    for(auto u : C){
//                        Pr_v.emplace_back(Pr[u]);
//                    }
//                    return result;
//                }
//            }
//        }
//
//        for(auto v : del_list) set_of_C.erase(v);

        vector<vector<int>> Gi(n);
        for(auto idx : exist_idx[i]){
            int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
//                --remain;
//                set_of_C.erase(u);
//                if(remain == 0) {
//                    for(auto v : C){
//                        Pr_v.emplace_back(Pr[v]);
//                    }
//                    return result;
//                }
            }
        }

        for(auto idx : exist_idx[i]){
            int idx_of_sample = geo_vs[idx](re)+i+1; // 采样数字可能是0，加1保证从下个采样图开始计算
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(idx);
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> ls_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                   vector<int> &C, int q, int k, double theta,
                   vector<double> &Pr_v, string &file, int N)
{
    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = theta;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();
    string outfile = "output/Sample2Resultsize_ls_"+to_string(q)+"_"+file;
    ofstream out(outfile);

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    random_device dev;
    default_random_engine re(dev());
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
    for(int i = 0; i < m; i++){
        int idx_of_sample = geo_vs[i](re);
        if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
        exist_idx[idx_of_sample].template emplace_back(i);
    }

    for(int i = 0; i < N; i++){
        vector<int> del_list;

        for(auto v : C){
            double remain_prob = (N - i) * 1.0 / N;
            if(Pr[v] + remain_prob < theta){
                --remain;
                del_list.emplace_back(v);
//                set_of_C.erase(v);
                if(remain == 0) {
                    for(auto u : C){
                        Pr_v.emplace_back(Pr[u]);
                    }
                    out<<i+1<<" "<<N<<" "<<result.size()<<endl;
                    cout<<i+1<<" "<<N<<" "<<result.size()<<endl;
                    return result;
                }
            }
        }

        for(auto v : del_list) set_of_C.erase(v);

        vector<vector<int>> Gi(n);
        for(auto idx : exist_idx[i]){
            int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
                --remain;
                set_of_C.erase(u);
                if(remain == 0) {
                    for(auto v : C){
                        Pr_v.emplace_back(Pr[v]);
                    }
                    out<<i+1<<" "<<N<<" "<<result.size()<<endl;
                    cout<<i+1<<" "<<N<<" "<<result.size()<<endl;
                    return result;
                }
            }
        }

        for(auto idx : exist_idx[i]){
            int idx_of_sample = geo_vs[idx](re)+i+1; // 采样数字可能是0，加1保证从下个采样图开始计算
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(idx);
        }
        out<<i+1<<" "<<N<<" "<<result.size()<<endl;
        cout<<i+1<<" "<<N<<" "<<result.size()<<endl;
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    out.close();
    return result;
}

vector<int> ls_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
               vector<int> &C, int q, int k, double theta,
               vector<double> &Pr_v, int r)
{
    // Parameters
    int N = 1000;
//    int r = 50;
    int k0 = k;
    double p0 = theta;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    random_device dev;
    default_random_engine re(dev());
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
    for(int i = 0; i < m; i++){
        int idx_of_sample = geo_vs[i](re);
        if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
        exist_idx[idx_of_sample].template emplace_back(i);
    }

    for(int i = 0; i < N; i++){
        vector<int> del_list;

        for(auto v : C){
            double remain_prob = (N - i) * 1.0 / N;
            if(Pr[v] + remain_prob < theta){
                --remain;
                del_list.emplace_back(v);
//                set_of_C.erase(v);
                if(remain == 0) {
                    for(auto u : C){
                        Pr_v.emplace_back(Pr[u]);
                    }
                    return result;
                }
            }
        }

        for(auto v : del_list) set_of_C.erase(v);

        vector<vector<int>> Gi(n);
        for(auto idx : exist_idx[i]){
            int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
                --remain;
                set_of_C.erase(u);
                if(remain == 0) {
                    for(auto v : C){
                        Pr_v.emplace_back(Pr[v]);
                    }
                    return result;
                }
            }
        }

        for(auto idx : exist_idx[i]){
            int idx_of_sample = geo_vs[idx](re)+i+1; // 采样数字可能是0，加1保证从下个采样图开始计算
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(idx);
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> ls_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                 vector<int> &C, int q, int k, double theta,
                 vector<double> &Pr_v, int N, int r)
{
    // Parameters
//    int N = 500;
//    int r = 50;
    int k0 = k;
    double p0 = theta;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    random_device dev;
    default_random_engine re(dev());
    vector<geometric_distribution<int>> geo_vs(m);
    for(int i = 0; i < m; i++){
        double e_prob = get<2>(Edges[i]); // 边上的概率
        geometric_distribution<int> temp(e_prob); // 几何分布的参数是成功的概率，即边上的概率
        geo_vs[i] = temp;
    }

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
    for(int i = 0; i < m; i++){
        int idx_of_sample = geo_vs[i](re);
        if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
        exist_idx[idx_of_sample].template emplace_back(i);
    }

    for(int i = 0; i < N; i++){
//        vector<int> del_list;
//
//        for(auto v : C){
//            double remain_prob = (N - i) * 1.0 / N;
//            if(Pr[v] + remain_prob < theta){
//                --remain;
//                del_list.emplace_back(v);
////                set_of_C.erase(v);
//                if(remain == 0) {
//                    for(auto u : C){
//                        Pr_v.emplace_back(Pr[u]);
//                    }
//                    return result;
//                }
//            }
//        }
//
//        for(auto v : del_list) set_of_C.erase(v);

        vector<vector<int>> Gi(n);
        for(auto idx : exist_idx[i]){
            int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
//                --remain;
//                set_of_C.erase(u);
//                if(remain == 0) {
//                    for(auto v : C){
//                        Pr_v.emplace_back(Pr[v]);
//                    }
//                    return result;
//                }
            }
        }

        for(auto idx : exist_idx[i]){
            int idx_of_sample = geo_vs[idx](re)+i+1; // 采样数字可能是0，加1保证从下个采样图开始计算
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(idx);
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

int main(void)
{
    string filename = "../data/smallp.txt";

    base b;
    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);
    printf("V=%d, E=%d\n", b.V, b.E);

    int q = 0;
    int k = 2;
    double theta = 1e-10;

    vector<int> C(b.V);
    iota(C.begin(), C.end(), 0);

    vector<double> Pr_v;

    START
    auto ans = ls(G, Edges, C, q, k, theta, Pr_v);
    END("LAZY")
    printf("size=%d\n", (int)ans.size());
}