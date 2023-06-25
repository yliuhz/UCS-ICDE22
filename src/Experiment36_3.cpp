//
// Created by liuyue on 21/4/2021.
// TIPS: 测时间，关于k
// 1520:0.8
//

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"
#include "../include/probabilistic_kcore_computation.h"
#include "../include/mcs_with_prune.h"
#include "../include/ss-vl.h"
#include "../include/ss-sc.h"
#include "../include/ls.h"
#include "../include/lssa-vl.h"
#include "../include/lssa-sc.h"

using namespace std;

int main(void)
{
    vector<string> files = {
            "email-Enron-p.txt",
            "flickr.txt",
            "dblp-graph-p-18-20.txt",
            "biomine.txt",
    };

//    vector<int> qs = {13, 54, 13, 555507};
//    vector<int> ks = {10, 30, 10, 134};

    vector<vector<vector<int>>> qs = {
            { // email-Enron
                    { // k = 40
                            27, 73, 102, 173, 193, 261, 271, 280, 281, 301,
                            312, 344, 406, 438, 472, 559, 568, 603, 615, 620,
                    }
            },
            { // Flickr
                    { // k = 40
                            709, 780, 807, 922, 2893, 2903, 3092, 3155, 3218, 3721,
                            3931, 3993, 4008, 4082, 4116, 4449, 4468, 4642, 4968, 4978,
                    },
            },
            { // DBLP
                    { // k = 40
                            692, 886, 1063, 1065, 1066, 1906, 2258, 4246, 6223, 6369,
                            10382, 10561, 10594, 15651, 16201, 16269, 18185, 19370, 24773, 25477,
                    },
            },
            { // Biomine
                    { // k = 40
                            109, 118, 218, 225, 387, 682, 1334, 1369, 1589, 1722,
                            1890, 2256, 2510, 2776, 2782, 2862, 3019, 3361, 3482, 3716,
                    },
            }
    };

    vector<vector<int>> ks = {
            {2,3,4,5,6,7,8,9,10},
            {2,3,4,5,6,7,8,9,10},
            {2,3,4,5,6,7,8,9,10},
            {2,3,4,5,6,7,8,9,10},
    };

    double theta = 0.8;

    base b;
    locals lo;
    pkcore pkc;

    clock_t start, end;
    double time; // 单位：秒

    for(int i = 2; i < 3; i++){ // TIPS:只测DBLP
        string file = files[i];
        string filename = "data/"+file;
        string outfile = "output/"+file+"_Experiment36(k2time).txt";

        printf("Filename: %s\n", file.c_str());
        printf("%s: Reading graph ...\n", file.c_str());

        auto G = b.readFilep(filename);
        auto Edges = b.readFileE(filename);
        auto dG = b.toDeter(G);

        auto ks_of_this_file = ks[i];
        auto qs_of_this_file = qs[i];

        unordered_map<int,vector<double>> k2time_mcs, k2time_ss_sc, k2time_ls, k2time_lssa_sc;

        printf("%s: Start evaluating ...\n", file.c_str());

        for(int l = 0; l < ks_of_this_file.size(); l++){
            int k = ks_of_this_file[l]; // TIPS: k从2开始

            auto qs_of_this_k = qs_of_this_file[0]; // TIPS: 选取core_number=40的顶点

            for(int ll = 0; ll < qs_of_this_k.size(); ll++){
                int q = qs_of_this_k[ll];

                auto Cqk = lo.global(q, k, dG);
                auto G_prune_i = lo.extractSubgraph(Cqk, G);
                G_prune_i = lo.rearrange(G_prune_i, q, Cqk);
                auto Edges_prune_i = lo.table2Edges(G_prune_i);
                auto dG_prune_i = b.toDeter(G_prune_i);

                vector<int> C;
                for(auto v : Cqk){
                    if(pkc.degGeK(G_prune_i, v, k) >= theta){
                        C.emplace_back(v);
                    }
                }

                printf("G size=%d, G_pruned size=%d, Edges size=%d, Edges_pruned size=%d, C size=%d\n",
                       (int)G.size(), (int)G_prune_i.size(), (int)Edges.size(), (int)Edges_prune_i.size(),
                       (int)C.size());

                printf("%s: %d/%d k, %d/%d q\n", file.c_str(),
                       l+1, (int)ks_of_this_file.size(),
                       ll+1, (int)qs_of_this_k.size());

                vector<int> ans;
                vector<double> Pr_v;

                start = clock();
                ans = mcs_with_prune(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v, 1000);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_mcs[k].emplace_back(time);
                printf("Time of MCS: %.2f s\n", time);

                start = clock();
                ans = ss_sc(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v, 1000);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_ss_sc[k].emplace_back(time);
                printf("Time of SS-SC: %.2f s\n", time);

                start = clock();
                ans = ls(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v, 1000);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_ls[k].emplace_back(time);
                printf("Time of LS: %.2f s\n", time);

                start = clock();
                ans = lssa_sc(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v, 1000);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_lssa_sc[k].emplace_back(time);
                printf("Time of LSSA-SC: %.2f s\n", time);
            }
        }

        printf("%s: Writing time result to file ...\n", file.c_str());

        ofstream out(outfile);

        out<<"MCS"<<endl;
        for(auto [k, time_of_qs] : k2time_mcs){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out<<"SS-SC"<<endl;
        for(auto [k, time_of_qs] : k2time_ss_sc){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out<<"LS"<<endl;
        for(auto [k, time_of_qs] : k2time_ls){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out<<"LSSA-SC"<<endl;
        for(auto [k, time_of_qs] : k2time_lssa_sc){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out.close();
    }
}