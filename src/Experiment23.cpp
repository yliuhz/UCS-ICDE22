//
// Created by liuyue on 11/5/2021.
//

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"
#include "../include/mcs_with_prune.h"
#include "../include/ss-sc.h"
#include "../include/ls.h"
#include "../include/lssa-sc.h"

using namespace std;

int main(void)
{
    string file = "email-Enron-p.txt";
    string filename = "data/"+file;
    string outfile = "output/n02var_"+file;

    int k = 5;
    double theta = 0.8;

    int q = 27; // TIPS:选取core_number=40的一个顶点
    int n0max = 200; // TIPS:最大n0
    int N = 500; // TIPS:采样数强制为250（测方差的实验保证采样数相同）

    base b;
    locals lo;

    auto G = b.readFilep(filename);
    auto Edges = b.readFileE(filename);

    auto Cqk = lo.global(q,k,G);
    auto G_pruned = lo.extractSubgraph(Cqk, G);
    G_pruned = lo.rearrange(G, q, Cqk);
    auto Edges_pruned = lo.table2Edges(G_pruned);
    auto dG_pruned = b.toDeter(G_pruned);


    vector<double> Pr_v;
    vector<vector<double>> Prs;
    vector<int> n02var_mcs, n02var_ss, n02var_ls, n02var_lssa;
    ofstream out(outfile);
    assert(!out.fail());

    printf("Start MCS\n");
    out<<"MCS"<<endl;
    Prs.clear();
    for(int n0=1; n0<=n0max; n0++){
        START
        mcs_with_prune_f(G_pruned, Edges_pruned, Cqk, q, k, theta, Pr_v, N);
        END("mcs")
        Prs.emplace_back(Pr_v);
        double var = b.var(Prs);
        out<<n0<<" "<<n0max<<" "<<var<<endl;
        cout<<n0<<" "<<n0max<<" "<<var<<endl;
    }

    printf("Start SS\n");
    out<<"SS"<<endl;
    Prs.clear();
    for(int n0=1; n0<=n0max; n0++){
        START
        ss_sc_f(G_pruned, Edges_pruned, Cqk, q, k, theta, Pr_v, N);
        END("ss")
        Prs.emplace_back(Pr_v);
        double var = b.var(Prs);
        out<<n0<<" "<<n0max<<" "<<var<<endl;
        cout<<n0<<" "<<n0max<<" "<<var<<endl;
    }

    printf("Start LS\n");
    out<<"LS"<<endl;
    Prs.clear();
    for(int n0=1; n0<=n0max; n0++){
        START
        ls_f(G_pruned, Edges_pruned, Cqk, q, k, theta, Pr_v, N);
        END("ls")
        Prs.emplace_back(Pr_v);
        double var = b.var(Prs);
        out<<n0<<" "<<n0max<<" "<<var<<endl;
        cout<<n0<<" "<<n0max<<" "<<var<<endl;
    }

    printf("Start LSSA\n");
    out<<"LSSA"<<endl;
    Prs.clear();
    for(int n0=1; n0<=n0max; n0++){
        START
        lssa_sc_f(G_pruned, Edges_pruned, Cqk, q, k, theta, Pr_v, N);
        END("lssa")
        Prs.emplace_back(Pr_v);
        double var = b.var(Prs);
        out<<n0<<" "<<n0max<<" "<<var<<endl;
        cout<<n0<<" "<<n0max<<" "<<var<<endl;
    }

    out.close();
}