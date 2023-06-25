//
// Created by liuyue on 19/4/2021.
//

#include "../include/lssa-sc.h"

vector<int> lssa_sc(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                    vector<int> &C, int q, int k, double theta,
                    vector<double> &Pr_v, int N, int r)
{
    // Parameters
//    int N = 500;
//    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;
//    for(int i = 0; i < Edges.size(); i++){
//        int u = get<0>(Edges[i]);
//        int v = get<1>(Edges[i]);
//        double prob = get<2>(Edges[i]);
//        if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//            swap(Edges[ridx++], Edges[i]);
//            if(ridx == r) break; // TIPS: 至多选择r条边
//        }
//    }
//
//    if(ridx == 0){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }

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

    double remain_Omega_prob = 1.0;

    for(int i = 0; i < ridx+1; i++){
        // basis中的范围:[0,ridx)
        // Omega_i: [0,i-1]选择，[i]丢弃,[i+1,ridx)采样

        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++) {
            pi *= get<2>(Edges[j]);
            int u = get<0>(Edges[j]), v = get<1>(Edges[j]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }
        if(i < ridx) pi *= 1 - get<2>(Edges[i]);

        int Ni = ceil(N * pi);

        vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
        for(int j = min(i+1,ridx); j < m; j++){
            int idx_of_sample = geo_vs[j](re);
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(j);
        }

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++) {
            vector<int> del_list;

            for(auto v : C){
                double remain_prob = remain_Omega_prob + (Ni - j) * 1.0 / Ni * pi;
                if(Pr[v] + remain_prob < theta){
                    del_list.emplace_back(v);
//                    set_of_C.erase(v);
                    --remain;
                    if(remain == 0) {
                        for(auto u : C){
                            Pr_v.emplace_back(Pr[u]);
                        }
                        return result;
                    }
                }
            }

            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi;
            for (auto idx : exist_idx[j]) {
                int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
                Gij[u].emplace_back(v);
                Gij[v].emplace_back(u);
            }

            auto Cqk = lo.global(q, k, Gij);
            for (auto u : Cqk) {
                Pr[u] += pi * 1.0 / Ni;
                if (Pr[u] - pi * 1.0 / Ni < theta && Pr[u] >= theta) {
                    result.emplace_back(u);
                    --remain;
                    set_of_C.erase(u);
                    if (remain == 0) {
                        for(auto v : C){
                            Pr_v.emplace_back(Pr[v]);
                        }
                        return result;
                    }
                }
            }

            for (auto idx : exist_idx[j]) {
                int idx_of_sample = geo_vs[idx](re) + j + 1; // 采样数字可能是0，加1保证从下个采样图开始计算
                if ((int) exist_idx.size() - 1 < idx_of_sample) exist_idx.resize(idx_of_sample + 1);
                exist_idx[idx_of_sample].template emplace_back(idx);
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }
    return result;
}

vector<int> lssa_sc_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                    vector<int> &C, int q, int k, double theta,
                    vector<double> &Pr_v, int N, int r)
{
    // Parameters
//    int N = 500;
//    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;
//    for(int i = 0; i < Edges.size(); i++){
//        int u = get<0>(Edges[i]);
//        int v = get<1>(Edges[i]);
//        double prob = get<2>(Edges[i]);
//        if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//            swap(Edges[ridx++], Edges[i]);
//            if(ridx == r) break; // TIPS: 至多选择r条边
//        }
//    }
//
//    if(ridx == 0){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }

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

    double remain_Omega_prob = 1.0;

    for(int i = 0; i < ridx+1; i++){
        // basis中的范围:[0,ridx)
        // Omega_i: [0,i-1]选择，[i]丢弃,[i+1,ridx)采样

        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++) {
            pi *= get<2>(Edges[j]);
            int u = get<0>(Edges[j]), v = get<1>(Edges[j]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }
        if(i < ridx) pi *= 1 - get<2>(Edges[i]);

        int Ni = ceil(N * pi);

        vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
        for(int j = min(i+1,ridx); j < m; j++){
            int idx_of_sample = geo_vs[j](re);
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(j);
        }

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++) {
//            vector<int> del_list;
//
//            for(auto v : C){
//                double remain_prob = remain_Omega_prob + (Ni - j) * 1.0 / Ni * pi;
//                if(Pr[v] + remain_prob < theta){
//                    del_list.emplace_back(v);
////                    set_of_C.erase(v);
//                    --remain;
//                    if(remain == 0) {
//                        for(auto u : C){
//                            Pr_v.emplace_back(Pr[u]);
//                        }
//                        return result;
//                    }
//                }
//            }
//
//            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi;
            for (auto idx : exist_idx[j]) {
                int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
                Gij[u].emplace_back(v);
                Gij[v].emplace_back(u);
            }

            auto Cqk = lo.global(q, k, Gij);
            for (auto u : Cqk) {
                Pr[u] += pi * 1.0 / Ni;
                if (Pr[u] - pi * 1.0 / Ni < theta && Pr[u] >= theta) {
                    result.emplace_back(u);
//                    --remain;
//                    set_of_C.erase(u);
//                    if (remain == 0) {
//                        for(auto v : C){
//                            Pr_v.emplace_back(Pr[v]);
//                        }
//                        return result;
//                    }
                }
            }

            for (auto idx : exist_idx[j]) {
                int idx_of_sample = geo_vs[idx](re) + j + 1; // 采样数字可能是0，加1保证从下个采样图开始计算
                if ((int) exist_idx.size() - 1 < idx_of_sample) exist_idx.resize(idx_of_sample + 1);
                exist_idx[idx_of_sample].template emplace_back(idx);
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }
    return result;
}

vector<int> lssa_sc_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                        vector<int> &C, int q, int k, double theta,
                        vector<double> &Pr_v, int N, int r)
{
    // Parameters
//    int N = 500;
//    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();
    string outfile = "output/Sample2Resultsize_lssa_sc.txt";
    ofstream out(outfile);

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;
//    for(int i = 0; i < Edges.size(); i++){
//        int u = get<0>(Edges[i]);
//        int v = get<1>(Edges[i]);
//        double prob = get<2>(Edges[i]);
//        if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//            swap(Edges[ridx++], Edges[i]);
//            if(ridx == r) break; // TIPS: 至多选择r条边
//        }
//    }
//
//    if(ridx == 0){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }

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

    double remain_Omega_prob = 1.0;
    int samplesize = 0;

    for(int i = 0; i < ridx+1; i++){
        // basis中的范围:[0,ridx)
        // Omega_i: [0,i-1]选择，[i]丢弃,[i+1,ridx)采样

        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++) {
            pi *= get<2>(Edges[j]);
            int u = get<0>(Edges[j]), v = get<1>(Edges[j]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }
        if(i < ridx) pi *= 1 - get<2>(Edges[i]);

        int Ni = ceil(N * pi);

        vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
        for(int j = min(i+1,ridx); j < m; j++){
            int idx_of_sample = geo_vs[j](re);
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(j);
        }

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++) {
            samplesize++;
            vector<int> del_list;

            for(auto v : C){
                double remain_prob = remain_Omega_prob + (Ni - j) * 1.0 / Ni * pi;
                if(Pr[v] + remain_prob < theta){
                    del_list.emplace_back(v);
//                    set_of_C.erase(v);
                    --remain;
                    if(remain == 0) {
                        for(auto u : C){
                            Pr_v.emplace_back(Pr[u]);
                        }
                        out<<samplesize<<" "<<N<<" "<<result.size()<<endl;
                        return result;
                    }
                }
            }

            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi;
            for (auto idx : exist_idx[j]) {
                int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
                Gij[u].emplace_back(v);
                Gij[v].emplace_back(u);
            }

            auto Cqk = lo.global(q, k, Gij);
            for (auto u : Cqk) {
                Pr[u] += pi * 1.0 / Ni;
                if (Pr[u] - pi * 1.0 / Ni < theta && Pr[u] >= theta) {
                    result.emplace_back(u);
                    --remain;
                    set_of_C.erase(u);
                    if (remain == 0) {
                        for(auto v : C){
                            Pr_v.emplace_back(Pr[v]);
                        }
                        out<<samplesize<<" "<<N<<" "<<result.size()<<endl;
                        return result;
                    }
                }
            }

            for (auto idx : exist_idx[j]) {
                int idx_of_sample = geo_vs[idx](re) + j + 1; // 采样数字可能是0，加1保证从下个采样图开始计算
                if ((int) exist_idx.size() - 1 < idx_of_sample) exist_idx.resize(idx_of_sample + 1);
                exist_idx[idx_of_sample].template emplace_back(idx);
            }
            out<<samplesize<<" "<<N<<" "<<result.size()<<endl;
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    out.close();
    return result;
}

vector<int> lssa_sc_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                    vector<int> &C, int q, int k, double theta,
                    vector<double> &Pr_v, int r)
{
    // Parameters
    int N = 1000;
//    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;
//    for(int i = 0; i < Edges.size(); i++){
//        int u = get<0>(Edges[i]);
//        int v = get<1>(Edges[i]);
//        double prob = get<2>(Edges[i]);
//        if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//            swap(Edges[ridx++], Edges[i]);
//            if(ridx == r) break; // TIPS: 至多选择r条边
//        }
//    }
//
//    if(ridx == 0){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }

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

    double remain_Omega_prob = 1.0;

    for(int i = 0; i < ridx+1; i++){
        // basis中的范围:[0,ridx)
        // Omega_i: [0,i-1]选择，[i]丢弃,[i+1,ridx)采样

        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++) {
            pi *= get<2>(Edges[j]);
            int u = get<0>(Edges[j]), v = get<1>(Edges[j]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }
        if(i < ridx) pi *= 1 - get<2>(Edges[i]);

        int Ni = ceil(N * pi);

        vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
        for(int j = min(i+1,ridx); j < m; j++){
            int idx_of_sample = geo_vs[j](re);
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(j);
        }

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++) {
            vector<int> del_list;

            for(auto v : C){
                double remain_prob = remain_Omega_prob + (Ni - j) * 1.0 / Ni * pi;
                if(Pr[v] + remain_prob < theta){
                    del_list.emplace_back(v);
//                    set_of_C.erase(v);
                    --remain;
                    if(remain == 0) {
                        for(auto u : C){
                            Pr_v.emplace_back(Pr[u]);
                        }
                        return result;
                    }
                }
            }

            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi;
            for (auto idx : exist_idx[j]) {
                int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
                Gij[u].emplace_back(v);
                Gij[v].emplace_back(u);
            }

            auto Cqk = lo.global(q, k, Gij);
            for (auto u : Cqk) {
                Pr[u] += pi * 1.0 / Ni;
                if (Pr[u] - pi * 1.0 / Ni < theta && Pr[u] >= theta) {
                    result.emplace_back(u);
                    --remain;
                    set_of_C.erase(u);
                    if (remain == 0) {
                        for(auto v : C){
                            Pr_v.emplace_back(Pr[v]);
                        }
                        return result;
                    }
                }
            }

            for (auto idx : exist_idx[j]) {
                int idx_of_sample = geo_vs[idx](re) + j + 1; // 采样数字可能是0，加1保证从下个采样图开始计算
                if ((int) exist_idx.size() - 1 < idx_of_sample) exist_idx.resize(idx_of_sample + 1);
                exist_idx[idx_of_sample].template emplace_back(idx);
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }
    return result;
}

vector<int> lssa_sc_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      vector<int> &C, int q, int k, double theta,
                      vector<double> &Pr_v, int N, int r)
{
    // Parameters
//    int N = 500;
//    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;
//    for(int i = 0; i < Edges.size(); i++){
//        int u = get<0>(Edges[i]);
//        int v = get<1>(Edges[i]);
//        double prob = get<2>(Edges[i]);
//        if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//            swap(Edges[ridx++], Edges[i]);
//            if(ridx == r) break; // TIPS: 至多选择r条边
//        }
//    }
//
//    if(ridx == 0){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }

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

    double remain_Omega_prob = 1.0;

    for(int i = 0; i < ridx+1; i++){
        // basis中的范围:[0,ridx)
        // Omega_i: [0,i-1]选择，[i]丢弃,[i+1,ridx)采样

        double pi = 1.0;
        vector<vector<int>> Gi(n);
        for(int j = 0; j < i; j++) {
            pi *= get<2>(Edges[j]);
            int u = get<0>(Edges[j]), v = get<1>(Edges[j]);
            Gi[u].emplace_back(v);
            Gi[v].emplace_back(u);
        }
        if(i < ridx) pi *= 1 - get<2>(Edges[i]);

        int Ni = ceil(N * pi);

        vector<vector<int>> exist_idx; // 存储边在哪个采样图中出现
        for(int j = min(i+1,ridx); j < m; j++){
            int idx_of_sample = geo_vs[j](re);
            if((int)exist_idx.size()-1 < idx_of_sample) exist_idx.resize(idx_of_sample+1);
            exist_idx[idx_of_sample].template emplace_back(j);
        }

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++) {
//            vector<int> del_list;
//
//            for(auto v : C){
//                double remain_prob = remain_Omega_prob + (Ni - j) * 1.0 / Ni * pi;
//                if(Pr[v] + remain_prob < theta){
//                    del_list.emplace_back(v);
////                    set_of_C.erase(v);
//                    --remain;
//                    if(remain == 0) {
//                        for(auto u : C){
//                            Pr_v.emplace_back(Pr[u]);
//                        }
//                        return result;
//                    }
//                }
//            }
//
//            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi;
            for (auto idx : exist_idx[j]) {
                int u = get<0>(Edges[idx]), v = get<1>(Edges[idx]);
                Gij[u].emplace_back(v);
                Gij[v].emplace_back(u);
            }

            auto Cqk = lo.global(q, k, Gij);
            for (auto u : Cqk) {
                Pr[u] += pi * 1.0 / Ni;
                if (Pr[u] - pi * 1.0 / Ni < theta && Pr[u] >= theta) {
                    result.emplace_back(u);
//                    --remain;
//                    set_of_C.erase(u);
//                    if (remain == 0) {
//                        for(auto v : C){
//                            Pr_v.emplace_back(Pr[v]);
//                        }
//                        return result;
//                    }
                }
            }

            for (auto idx : exist_idx[j]) {
                int idx_of_sample = geo_vs[idx](re) + j + 1; // 采样数字可能是0，加1保证从下个采样图开始计算
                if ((int) exist_idx.size() - 1 < idx_of_sample) exist_idx.resize(idx_of_sample + 1);
                exist_idx[idx_of_sample].template emplace_back(idx);
            }
        }
    }

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
    auto dG = b.toDeter(G);
    printf("V=%d, E=%d\n", b.V, b.E);

    int q = 0;
    int k = 2;
    double theta = 0.5;

    vector<int> C(b.V);
    iota(C.begin(), C.end(), 0);

    vector<double> Pr_v;

    START
    auto ans = lssa_sc(G, Edges, C, q, k, theta, Pr_v);
    END("LSSA-SC")
    printf("size=%d\n", (int)ans.size());
}