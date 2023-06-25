//
// Created by liuyue on 21/4/2021.
// TIPS: 测时间，关于k
// 1522:0.7
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
                    { // k = 5
                            16, 237, 270, 272, 321, 468, 571, 609, 648, 649,
                            701, 702, 722, 763, 911, 912, 916, 942, 993, 1023,
                    },
                    { // k = 10
                            1, 7, 13, 55, 317, 330, 348, 397, 447, 476,
                            503, 718, 731, 746, 765, 821, 1029, 1135, 1136, 1150,
                    },
                    { // k = 15
                            96, 297, 350, 389, 405, 413, 496, 629, 660, 695,
                            705, 755, 789, 1149, 1211, 1311, 1328, 1352, 1596, 1866,
                    },
                    { // k = 20
                            217, 226, 506, 529, 541, 665, 709, 800, 801, 959,
                            1089, 1129, 1175, 1290, 1331, 1364, 1429, 1483, 1484, 1485,
                    },
                    { // k = 25
                            178, 331, 379, 548, 654, 799, 814, 838, 898, 930,
                            933, 951, 1177, 1204, 1246, 1361, 1362, 1465, 1477, 1550,
                    },
                    { // k = 30
                            112, 246, 309, 377, 381, 518, 535, 536, 711, 730,
                            751, 758, 895, 937, 1034, 1128, 1173, 1238, 1289, 1302,
                    },
                    { // k = 35
                            82, 138, 197, 206, 307, 320, 500, 545, 547, 602,
                            656, 667, 704, 736, 766, 798, 822, 859, 876, 880,
                    },
                    { // k = 40
                            27, 73, 102, 173, 193, 261, 271, 280, 281, 301,
                            312, 344, 406, 438, 472, 559, 568, 603, 615, 620,
                    }
            },

    };

    vector<vector<int>> ks = {
            {5,10,15,20,25,30,35,40},
            {5,10,15,20,25,30,35,40,45,50},
            {5,10,15,20,25,30,35,40,45,50},
            {5,10,15,20,25,30,35,40,45,50},
    };

    double theta = 0.7;

    base b;
    locals lo;
    pkcore pkc;

    clock_t start, end;
    double time; // 单位：秒

    for(int i = 0; i < files.size(); i++){
        string file = files[i];
        string filename = "data/"+file;
        string outfile = "output/"+file+"_Experiment2(time).txt";

        printf("Filename: %s\n", file.c_str());
        printf("%s: Reading graph ...\n", file.c_str());

        auto G = b.readFilep_from_exist(filename);
        auto Edges = b.readFileE(filename);
        auto dG = b.toDeter(G);

        auto ks_of_this_file = ks[i];
        auto qs_of_this_file = qs[i];

        unordered_map<int,vector<double>> k2time_mcs, k2time_ss_vl, k2time_ss_sc, k2time_ls;

        printf("%s: Start evaluating ...\n", file.c_str());

        for(int l = 0; l < ks_of_this_file.size(); l++){
            int k = ks_of_this_file[l];

            auto qs_of_this_k = qs_of_this_file[l];

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
                ans = mcs_with_prune(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_mcs[k].emplace_back(time);
                printf("Time of MCS: %.2f s\n", time);

                start = clock();
                ans = ss_vl(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_ss_vl[k].emplace_back(time);
                printf("Time of SS-VL: %.2f s\n", time);

                start = clock();
                ans = ss_sc(G_prune_i, Edges_prune_i, dG, C, q, k, theta, Pr_v);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_ss_sc[k].emplace_back(time);
                printf("Time of SS-SC: %.2f s\n", time);

                start = clock();
                ans = ls(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
                end = clock();
                time = (double)(end-start)/CLOCKS_PER_SEC;
                k2time_ls[k].emplace_back(time);
                printf("Time of LS: %.2f s\n", time);
            }
        }

        printf("%s: Writing time result to file ...\n", file.c_str());

        ofstream out(outfile);

        out<<"MCS"<<endl;
        for(auto [k, time_of_qs] : k2time_mcs){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<ios::fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out<<"SS-VL"<<endl;
        for(auto [k, time_of_qs] : k2time_ss_vl){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<ios::fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out<<"SS-SC"<<endl;
        for(auto [k, time_of_qs] : k2time_ss_sc){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<ios::fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out<<"LS"<<endl;
        for(auto [k, time_of_qs] : k2time_ls){
            out<<"k="<<k<<" ,qn="<<time_of_qs.size();
            for(int j = 0; j < time_of_qs.size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<ios::fixed<<setprecision(3)<<time_of_qs[j]<<" ";
            }
            out<<endl;
        }

        out.close();
    }
}