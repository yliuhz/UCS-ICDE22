//
// Created by liuyue on 20/4/2021.
//

#include "../include/mcs_with_prune.h"

vector<int> mcs_with_prune(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                           vector<int> &C, int q, int k, double theta,
                           vector<double> &Pr_v, int N)
{
    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < N; i++){
        vector<int> del_list;

        for(auto v : set_of_C){
            double remain_prob = (N - i) * 1.0 / N;
            if(Pr[v] + remain_prob < theta) {
                del_list.emplace_back(v);
//                set_of_C.erase(v);
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

        vector<vector<int>> Gi(n);

        for(int j = 0; j < m; j++){
            double prob = get<2>(Edges[j]);
            double random_prob = urd(re);
            if(prob >= random_prob){
                int u = get<0>(Edges[j]);
                int v = get<1>(Edges[j]);
                Gi[u].emplace_back(v);
                Gi[v].emplace_back(u);
            }
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){ // TIPS: 未判断是否属于set_of_C
                result.emplace_back(u);
                --remain;
                set_of_C.erase(u);
                if(remain == 0){
                    for(auto v : C){
                        Pr_v.emplace_back(Pr[v]);
                    }
                    return result;
                }
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> mcs_with_prune_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                           vector<int> &C, int q, int k, double theta,
                           vector<double> &Pr_v, int N)
{
    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear(); // ASSERT: Pr_v要清除重写

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < N; i++){
//        vector<int> del_list;
//
//        for(auto v : set_of_C){
//            double remain_prob = (N - i) * 1.0 / N;
//            if(Pr[v] + remain_prob < theta) {
//                del_list.emplace_back(v);
////                set_of_C.erase(v);
//                --remain;
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

        for(int j = 0; j < m; j++){
            double prob = get<2>(Edges[j]);
            double random_prob = urd(re);
            if(prob >= random_prob){
                int u = get<0>(Edges[j]);
                int v = get<1>(Edges[j]);
                Gi[u].emplace_back(v);
                Gi[v].emplace_back(u);
            }
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
//                --remain;
//                set_of_C.erase(u);
//                if(remain == 0){
//                    for(auto v : C){
//                        Pr_v.emplace_back(Pr[v]);
//                    }
//                    return result;
//                }
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> mcs_with_prune_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                               vector<int> &C, int q, int k, double theta,
                               vector<double> &Pr_v, string &file, int N)
{

    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();
    string outfile = "output/Sample2Resultsize_mcs_"+to_string(q)+"_"+file;
    ofstream out(outfile);

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < N; i++){
        vector<int> del_list;

        for(auto v : set_of_C){
            double remain_prob = (N - i) * 1.0 / N;
            if(Pr[v] + remain_prob < theta) {
                del_list.emplace_back(v);
//                set_of_C.erase(v);
                --remain;
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

        for(int j = 0; j < m; j++){
            double prob = get<2>(Edges[j]);
            double random_prob = urd(re);
            if(prob >= random_prob){
                int u = get<0>(Edges[j]);
                int v = get<1>(Edges[j]);
                Gi[u].emplace_back(v);
                Gi[v].emplace_back(u);
            }
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
                --remain;
                set_of_C.erase(u);
                if(remain == 0){
                    for(auto v : C){
                        Pr_v.emplace_back(Pr[v]);
                    }
                    out<<i+1<<" "<<N<<" "<<result.size()<<endl;
                    cout<<i+1<<" "<<N<<" "<<result.size()<<endl;
                    return result;
                }
            }
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

vector<int> mcs_with_prune_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
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

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < N; i++){
        vector<int> del_list;

        for(auto v : set_of_C){
            double remain_prob = (N - i) * 1.0 / N;
            if(Pr[v] + remain_prob < theta) {
                del_list.emplace_back(v);
//                set_of_C.erase(v);
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

        vector<vector<int>> Gi(n);

        for(int j = 0; j < m; j++){
            double prob = get<2>(Edges[j]);
            double random_prob = urd(re);
            if(prob >= random_prob){
                int u = get<0>(Edges[j]);
                int v = get<1>(Edges[j]);
                Gi[u].emplace_back(v);
                Gi[v].emplace_back(u);
            }
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){ // TIPS: 未判断是否属于set_of_C
                result.emplace_back(u);
                --remain;
                set_of_C.erase(u);
                if(remain == 0){
                    for(auto v : C){
                        Pr_v.emplace_back(Pr[v]);
                    }
                    return result;
                }
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> mcs_with_prune_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
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
    Pr_v.clear(); // ASSERT: Pr_v要清除重写

    // Implement
    if(C.empty()) return vector<int>{};
    int remain = C.size();
    unordered_set<int> set_of_C(C.begin(), C.end());

    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < N; i++){
//        vector<int> del_list;
//
//        for(auto v : set_of_C){
//            double remain_prob = (N - i) * 1.0 / N;
//            if(Pr[v] + remain_prob < theta) {
//                del_list.emplace_back(v);
////                set_of_C.erase(v);
//                --remain;
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

        for(int j = 0; j < m; j++){
            double prob = get<2>(Edges[j]);
            double random_prob = urd(re);
            if(prob >= random_prob){
                int u = get<0>(Edges[j]);
                int v = get<1>(Edges[j]);
                Gi[u].emplace_back(v);
                Gi[v].emplace_back(u);
            }
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){
                result.emplace_back(u);
//                --remain;
//                set_of_C.erase(u);
//                if(remain == 0){
//                    for(auto v : C){
//                        Pr_v.emplace_back(Pr[v]);
//                    }
//                    return result;
//                }
            }
        }
    }

    for(auto v : C){
        Pr_v.emplace_back(Pr[v]);
    }

    return result;
}

vector<int> baseline(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                     int q, int k, double theta,
                     vector<double> &Pr_v, int N)
{
    // Parameters
//    int N = 500;
    int r = 50;
    int k0 = k;
    double p0 = 0.5;
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear();

    // Implement
    uniform_real_distribution<double> urd(0.0, 1.0);
    random_device dev;
    default_random_engine re(dev());

    unordered_map<int,double> Pr;
    vector<int> result;
    locals lo;

    for(int i = 0; i < N; i++){

        vector<vector<int>> Gi(n);

        for(int j = 0; j < m; j++){
            double prob = get<2>(Edges[j]);
            double random_prob = urd(re);
            if(prob >= random_prob){
                int u = get<0>(Edges[j]);
                int v = get<1>(Edges[j]);
                Gi[u].emplace_back(v);
                Gi[v].emplace_back(u);
            }
        }

        auto Cqk = lo.global(q, k, Gi);
        for(auto u : Cqk){
            Pr[u] += 1.0 / N;
            if(Pr[u] - 1.0 / N < theta && Pr[u] >= theta){ // TIPS: 未判断是否属于set_of_C
                result.emplace_back(u);
            }
        }
    }

    for(int i = 0; i < n; i++){
        Pr_v.emplace_back(Pr[i]);
    }

    return result;
}

int main(void)
{
//    string filename = "../data/email-Enron-p.txt";
//    base b;
//    auto G = b.readFilep_from_exist(filename);
//    auto Edges = b.readFileE(filename);
//
//    printf("V=%d,E=%d\n", b.V, b.E);
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
//    auto ans = mcs_with_prune(G, Edges, C, q, k, theta, Pr_v);
//    END("MCS_WITH_PRUNE")
//    cout<<"size="<<ans.size()<<endl;
//
//    cout<<*max_element(Pr_v.begin(), Pr_v.end())<<endl;
}