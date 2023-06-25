//
// Created by liuyue on 19/4/2021.
//

#include "../include/ss-sc.h"
#include "../include/probabilistic_kcore_computation.h"

vector<int> ss_sc(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
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

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;

//    if(small_community.empty()){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }
//    else{
//        for(int i = 0; i < Edges.size(); i++){
//            int u = get<0>(Edges[i]);
//            int v = get<1>(Edges[i]);
//            double prob = get<2>(Edges[i]);
//            if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//                swap(Edges[ridx++], Edges[i]);
//                if(ridx == r) break; // TIPS: 至多选择r条边
//            }
//        }
//    }

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

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

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++){
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

            vector<vector<int>> Gij = Gi; // 第i层中的第j个采样图

            for(int l = i+1; l < Edges.size(); l++){
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
                    set_of_C.erase(u);
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

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> ss_sc_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
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

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;

//    if(small_community.empty()){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }
//    else{
//        for(int i = 0; i < Edges.size(); i++){
//            int u = get<0>(Edges[i]);
//            int v = get<1>(Edges[i]);
//            double prob = get<2>(Edges[i]);
//            if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//                swap(Edges[ridx++], Edges[i]);
//                if(ridx == r) break; // TIPS: 至多选择r条边
//            }
//        }
//    }

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

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

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++){
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

//            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi; // 第i层中的第j个采样图

            for(int l = i+1; l < Edges.size(); l++){
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
//                    --remain;
//                    set_of_C.erase(u);
//                    if(remain == 0) {
//                        for(auto v : C){
//                            Pr_v.emplace_back(Pr[v]);
//                        }
//                        return result;
//                    }
                }
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> ss_sc_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      vector<int> &C, int q, int k, double theta,
                      vector<double> &Pr_v, string &file, int N, int r)
{
    // Parameters
//    int N = 500;
//    int r = 50;
    int k0 = k;
    double p0 = theta;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();
    string outfile = "output/Sample2Resultsize_ss_sc_"+to_string(q)+"_"+file;
    ofstream out(outfile);

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;

//    if(small_community.empty()){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }
//    else{
//        for(int i = 0; i < Edges.size(); i++){
//            int u = get<0>(Edges[i]);
//            int v = get<1>(Edges[i]);
//            double prob = get<2>(Edges[i]);
//            if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//                swap(Edges[ridx++], Edges[i]);
//                if(ridx == r) break; // TIPS: 至多选择r条边
//            }
//        }
//    }

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

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

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++){
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
                        cout<<samplesize<<" "<<N<<" "<<result.size()<<endl;
                        return result;
                    }
                }
            }

            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi; // 第i层中的第j个采样图

            for(int l = i+1; l < Edges.size(); l++){
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
                    set_of_C.erase(u);
                    if(remain == 0) {
                        for(auto v : C){
                            Pr_v.emplace_back(Pr[v]);
                        }
                        out<<samplesize<<" "<<N<<" "<<result.size()<<endl;
                        cout<<samplesize<<" "<<N<<" "<<result.size()<<endl;
                        return result;
                    }
                }
            }
            out<<samplesize<<" "<<N<<" "<<result.size()<<endl;
            cout<<samplesize<<" "<<N<<" "<<result.size()<<endl;
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    out.close();
    return result;
}

vector<int> ss_sc_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
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

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;

//    if(small_community.empty()){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }
//    else{
//        for(int i = 0; i < Edges.size(); i++){
//            int u = get<0>(Edges[i]);
//            int v = get<1>(Edges[i]);
//            double prob = get<2>(Edges[i]);
//            if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//                swap(Edges[ridx++], Edges[i]);
//                if(ridx == r) break; // TIPS: 至多选择r条边
//            }
//        }
//    }

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

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

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++){
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

            vector<vector<int>> Gij = Gi; // 第i层中的第j个采样图

            for(int l = i+1; l < Edges.size(); l++){
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
                    set_of_C.erase(u);
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

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> ss_sc_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
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

    locals lo;
//    auto small_community = lo.local_li(q, k, dG, n, m);
//    unordered_set<int> sc_set(small_community.begin(), small_community.end());

    int ridx = r;

//    if(small_community.empty()){
//        cerr<<"BASIS is empty!!!"<<endl;
//        return vector<int>{-1};
//    }
//    else{
//        for(int i = 0; i < Edges.size(); i++){
//            int u = get<0>(Edges[i]);
//            int v = get<1>(Edges[i]);
//            double prob = get<2>(Edges[i]);
//            if(sc_set.find(u) != sc_set.end() && sc_set.find(v) != sc_set.end()){
//                swap(Edges[ridx++], Edges[i]);
//                if(ridx == r) break; // TIPS: 至多选择r条边
//            }
//        }
//    }

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

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

        remain_Omega_prob -= pi;

        for(int j = 0; j < Ni; j++){
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

//            for(auto v : del_list) set_of_C.erase(v);

            vector<vector<int>> Gij = Gi; // 第i层中的第j个采样图

            for(int l = i+1; l < Edges.size(); l++){
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
//                    --remain;
//                    set_of_C.erase(u);
//                    if(remain == 0) {
//                        for(auto v : C){
//                            Pr_v.emplace_back(Pr[v]);
//                        }
//                        return result;
//                    }
                }
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
//    string filename = "../data/email-Enron-p.txt";
//
//    base b;
//    auto G = b.readFilep(filename);
//    auto Edges = b.readFileE(filename);
//    auto dG = b.toDeter(G);
//    printf("V=%d, E=%d\n", b.V, b.E);
//
//    int q = 21;
//    int k = 3;
//    double theta = 0.8;
//
//    locals lo;
//    pkcore pkc;
//
//    auto Cqk = lo.global(q, k, G);
//
//    vector<int> C;
//    for(auto v : Cqk){
//        if(pkc.degGeK(G, v, k)>=theta) C.emplace_back(v);
//    }
//
//    vector<double> Pr_v;
//
//    START
//    auto ans = ss_sc(G, Edges, dG, C, q, k, theta, Pr_v);
//    END("SS-SC")
//    printf("size=%d\n", (int)ans.size());
//
//    cout<<*max_element(Pr_v.begin(), Pr_v.end())<<endl;
}
