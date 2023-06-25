// 直接采样+global

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"
#include "../include/baseline.h"

vector<vector<int>> baseline::sampleG(vector<tuple<int,int,double>> &Edges, int n, default_random_engine &re)
{
    vector<vector<int>> ans(n);
    uniform_real_distribution<double> urd(0.0, 1.0);
    for(auto e : Edges){
//        double prob = rand() / (double)RAND_MAX;
        double prob = urd(re);
        double e_prob = get<2>(e);
        if(e_prob >= prob){
            int x = get<0>(e), y = get<1>(e);
            ans[x].emplace_back(y);
            ans[y].emplace_back(x);
        }
    }
    return ans;
}

int nsamples(int n, double epsilon, double delta)
{
    int s = ceil(1.0/2/epsilon/epsilon*log(2*n/delta));
    return s;
}

vector<int> baseline::basef(vector<tuple<int,int,double>> &Edges,
                            int n, int q, int k, double theta)
{
    double epsilon = 0.1;
    double delta = 0.1;
    int s = nsamples(n, epsilon, delta);
    s = 1000;
    cout<<"s="<<s<<endl;

    locals ls;
    vector<int> nums(n);
    random_device dev;
    default_random_engine re(dev());
    for(int i = 0; i < s; i++){
        vector<vector<int>> G = sampleG(Edges, n, re); // n == 顶点个数
        vector<int> temp = ls.global(q, k, G);
        for(auto v : temp){
            nums[v]++;
        }
//        printf("i= %d/%d\n", i, s);
    }

    vector<int> ans;
    for(int i = 0; i < n; i++){
        double temp = nums[i] * 1.0 / s;
        if(temp >= theta) ans.emplace_back(i);
    }

//    for(int i = 0; i < n; i++){
//        cout<<"i="<<i<<",uc="<<nums[i]*1.0/s<<endl;
//    }

    return ans;
}

vector<tuple<int,int,double>> baseline::toDeter(vector<tuple<int, int, double>> &Edges, int l) {
    vector<tuple<int,int,double>> ans = Edges;
    for(auto &e : ans){
        get<2>(e) *= l;
    }
    return ans;
}

vector<tuple<int,int,double>> baseline::seg(vector<tuple<int,int,double>> &Edges, int l)
{
    vector<tuple<int,int,double>> ans = Edges;
    double unit = 1.0 / l;
    for(auto &e : ans){
        double prob = get<2>(e);
        get<2>(e) = ceil(prob / unit) * unit;
    }
    return ans;
}


int main(void)
{
    srand(20);

    string filename = "../data/OCnodeslinks.txt.txt";
    base b;
    auto G = b.readFilep_from_exist(filename);
    auto Edges = b.readFileE(filename);
    printf("V=%d, E=%d\n", b.V, b.E);
    baseline bs;
    int q = 480;
    int k = 10;
    double theta = 0.5;

    cout<<"q="<<q<<",k="<<k<<",theta="<<theta<<endl;

    locals lo;
    auto C = lo.global(q, k, G);
    auto H = lo.extractSubgraph(C, G);

    H = lo.rearrange(H, q);
    auto Edges_H = lo.table2Edges(H);

    cout<<"Max C size="<<C.size()<<",H size="<<H.size()<<",Edges_H size="<<Edges_H.size()<<endl;

    START
    vector<int> ans = bs.basef(Edges_H, G.size(), q, k, theta);
    END("BASELINE")
    printf("C size: %d\n", (int)ans.size());

//    locals ls;
//    auto DG = b.readFile(filename);
//    START
//    ans = ls.global(q, k, DG);
//    END("DETER GLOBAL")
//    printf("C size: %d\n", (int)ans.size());

//    START
//    auto seg = bs.seg(Edges, 4);
//    ans = bs.basef(seg, b.V, q, k, theta);
//    END("BASELINE2SEG")
//    b.printg(seg);
//    printf("C size: %d\n", (int)ans.size());
}