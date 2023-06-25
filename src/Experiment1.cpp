//
// Created by liuyue on 20/4/2021.
// TIPS: 测方差，随机选择qn个查询点
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

double norm2(vector<double> &v1, vector<double> &v2){
    double ret = 0.0;
    int n = v1.size();
    assert(v2.size() == n);
    for(int i = 0; i < n; i++){
        ret += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(ret);
}

int main(void)
{
    // Parameters
    int qn = 20;
    int n0 = 20; // 100
    double theta = 0.3;

    vector<string> files = {
            "email-Enron-p.txt",
            "flickr.txt",
            "dblp-graph-p-18-20.txt",
            "biomine.txt",
    };

    base b;
    locals lo;
    pkcore pkc;

    for(auto filename : files){
        string file = filename;
        filename = "../data/" + filename;
        auto G = b.readFilep_from_exist(filename);
        auto Edges = b.readFileE(filename);
        auto dG = b.toDeter(G);

        printf("Filename: %s\n", filename.c_str());
        printf("%s: V=%d, E=%d\n", file.c_str(), b.V, b.E);

        vector<int> ks = {5};
        vector<vector<int>> qs(ks.size());

        uniform_int_distribution<int> urd(0, b.V);
        random_device dev;
        default_random_engine re(dev());

        printf("%s: Generating %d queries ... \n", file.c_str(), qn);
        for(int i = 0; i < ks.size(); i++){
            for(int j = 0; j < qn; j++){
                qs[i].emplace_back(urd(re));
            }
        }

        printf("%s: Writing queries to file ... \n", file.c_str());
        string outfile = "output/query_vertices_of_" + file;
        ofstream out(outfile);
        for(int i = 0; i < ks.size(); i++){
            out<<"k="<<ks[i];
            for(int j = 0; j < qn; j++){
                if(j % 10 == 0) out<<endl;
                out<<qs[i][j]<<" ";
            }
            out<<endl;
        }
        out.close();

        printf("%s: Start evaluating ... \n", file.c_str());
        for(int i = 0; i < ks.size(); i++){
            int k = ks[i];

            vector<double> vars_mcs, vars_ss_vl, vars_ss_sc, vars_ls, vars_lssa_vl, vars_lssa_sc;

            for(int j = 0; j < qn; j++){
                int q = qs[i][j];
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

                vector<vector<double>> Prs;
                vector<double> Pr_v_avg;
                double var;

                // TIPS: MCS with prune(baseline)
                printf("%s: %d/%dth k, %d/%dth q, mcs\n", file.c_str(), i+1, (int)ks.size(), j+1, qn);
                for(int l = 0; l < n0; l++){
                    printf("%s:  %d/%dth k, %d/%dth q, %d/%d loop, mcs\n",
                           file.c_str(), i+1, (int)ks.size(), j+1, qn, l+1, n0);
                    vector<double> Pr_v;
                    START
                    mcs_with_prune(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
                    END("MCS")
                    if(Pr_v_avg.empty()) Pr_v_avg = Pr_v;
                    else{
                        for(int ll = 0; ll < Pr_v.size(); ll++){
                            Pr_v_avg[ll] += Pr_v[ll];
                        }
                    }
                    Prs.emplace_back(Pr_v);
                }

                for(int l = 0; l < Pr_v_avg.size(); l++){
                    Pr_v_avg[l] /= n0;
                }

                var = 0.0;
                for(int l = 0; l < n0; l++){
                    var += norm2(Prs[l], Pr_v_avg);
                }
                var /= n0 - 1;

                vars_mcs.emplace_back(var);

                // TIPS: SS_VL
                printf("%s: %d/%dth k, %d/%dth q, ss-vl\n", file.c_str(), i+1, (int)ks.size(), j+1, qn);
                Pr_v_avg.clear();
                for(int l = 0; l < n0; l++){
                    printf("%s:  %d/%dth k, %d/%dth q, %d/%d loop, ss-vl\n",
                           file.c_str(), i+1, (int)ks.size(), j+1, qn, l+1, n0);
                    vector<double> Pr_v;
                    START
                    ss_vl(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
                    END("SS-VL")
                    if(Pr_v_avg.empty()) Pr_v_avg = Pr_v;
                    else{
                        for(int ll = 0; ll < Pr_v.size(); ll++){
                            Pr_v_avg[ll] += Pr_v[ll];
                        }
                    }
                    Prs.emplace_back(Pr_v);
                }

                for(int l = 0; l < Pr_v_avg.size(); l++){
                    Pr_v_avg[l] /= n0;
                }

                var = 0.0;
                for(int l = 0; l < n0; l++){
                    var += norm2(Prs[l], Pr_v_avg);
                }
                var /= n0 - 1;

                vars_ss_vl.emplace_back(var);

                // TIPS: SS_SC
                printf("%s: %d/%dth k, %d/%dth q, ss-sc\n", file.c_str(), i+1, (int)ks.size(), j+1, qn);
                Pr_v_avg.clear();
                for(int l = 0; l < n0; l++){
                    printf("%s:  %d/%dth k, %d/%dth q, %d/%d loop, ss-sc\n",
                           file.c_str(), i+1, (int)ks.size(), j+1, qn, l+1, n0);
                    vector<double> Pr_v;
                    START
                    ss_sc(G_prune_i, Edges_prune_i, dG_prune_i, C, q, k, theta, Pr_v);
                    END("SS-SC")
                    if(Pr_v_avg.empty()) Pr_v_avg = Pr_v;
                    else{
                        for(int ll = 0; ll < Pr_v.size(); ll++){
                            Pr_v_avg[ll] += Pr_v[ll];
                        }
                    }
                    Prs.emplace_back(Pr_v);
                }

                for(int l = 0; l < Pr_v_avg.size(); l++){
                    Pr_v_avg[l] /= n0;
                }

                var = 0.0;
                for(int l = 0; l < n0; l++){
                    var += norm2(Prs[l], Pr_v_avg);
                }
                var /= n0 - 1;

                vars_ss_sc.emplace_back(var);

                // TIPS: LS
                printf("%s: %d/%dth k, %d/%dth q, ls\n", file.c_str(), i+1, (int)ks.size(), j+1, qn);
                Pr_v_avg.clear();
                for(int l = 0; l < n0; l++){
                    printf("%s:  %d/%dth k, %d/%dth q, %d/%d loop, ls\n",
                           file.c_str(), i+1, (int)ks.size(), j+1, qn, l+1, n0);
                    vector<double> Pr_v;
                    START
                    ls(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
                    END("LS")
                    if(Pr_v_avg.empty()) Pr_v_avg = Pr_v;
                    else{
                        for(int ll = 0; ll < Pr_v.size(); ll++){
                            Pr_v_avg[ll] += Pr_v[ll];
                        }
                    }
                    Prs.emplace_back(Pr_v);
                }

                for(int l = 0; l < Pr_v_avg.size(); l++){
                    Pr_v_avg[l] /= n0;
                }

                var = 0.0;
                for(int l = 0; l < n0; l++){
                    var += norm2(Prs[l], Pr_v_avg);
                }
                var /= n0 - 1;

                vars_ls.emplace_back(var);
//
//                // TIPS: LSSA-VL
//                printf("%s: %d/%dth k, %d/%dth q, lssa-vl\n", file.c_str(), i+1, (int)ks.size(), j+1, qn);
//                Pr_v_avg.clear();
//                for(int l = 0; l < n0; l++){
//                    printf("%s:  %d/%dth k, %d/%dth q, %d/%d loop, lssa-vl\n",
//                           file.c_str(), i+1, (int)ks.size(), j+1, qn, l+1, n0);
//                    vector<double> Pr_v;
//                    START
//                    lssa_vl(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v);
//                    END("LSSA-VL")
//                    if(Pr_v_avg.empty()) Pr_v_avg = Pr_v;
//                    else{
//                        for(int ll = 0; ll < Pr_v.size(); ll++){
//                            Pr_v_avg[ll] += Pr_v[ll];
//                        }
//                    }
//                    Prs.emplace_back(Pr_v);
//                }
//
//                for(int l = 0; l < Pr_v_avg.size(); l++){
//                    Pr_v_avg[l] /= n0;
//                }
//
//                var = 0.0;
//                for(int l = 0; l < n0; l++){
//                    var += norm2(Prs[l], Pr_v_avg);
//                }
//                var /= n0 - 1;
//
//                vars_lssa_vl.emplace_back(var);
//
//                // TIPS: LSSA-SC
//                printf("%s: %d/%dth k, %d/%dth q, lssa-sc\n", file.c_str(), i+1, (int)ks.size(), j+1, qn);
//                Pr_v_avg.clear();
//                for(int l = 0; l < n0; l++){
//                    printf("%s:  %d/%dth k, %d/%dth q, %d/%d loop, lssa-sc\n",
//                           file.c_str(), i+1, (int)ks.size(), j+1, qn, l+1, n0);
//                    vector<double> Pr_v;
//                    START
//                    lssa_sc(G_prune_i, Edges_prune_i, dG_prune_i, C, q, k, theta, Pr_v);
//                    END("LSSA-SC")
//                    if(Pr_v_avg.empty()) Pr_v_avg = Pr_v;
//                    else{
//                        for(int ll = 0; ll < Pr_v.size(); ll++){
//                            Pr_v_avg[ll] += Pr_v[ll];
//                        }
//                    }
//                    Prs.emplace_back(Pr_v);
//                }
//
//                for(int l = 0; l < Pr_v_avg.size(); l++){
//                    Pr_v_avg[l] /= n0;
//                }
//
//                var = 0.0;
//                for(int l = 0; l < n0; l++){
//                    var += norm2(Prs[l], Pr_v_avg);
//                }
//                var /= n0 - 1;
//
//                vars_lssa_sc.emplace_back(var);
            }

            double var_avg;

            // TIPS: 将方差结果写入文件, MCS
            printf("%s: Writing variances to file, mcs ... \n", file.c_str());
            out.open("../output/mcs_variances_of_" + file, ios::app);
            out<<"k="<<k;
            var_avg = 0.0;
            for(int j = 0; j < qn; j++){
                if(j % 10 == 0) out<<endl;
                out<<vars_mcs[j]<<" ";
                var_avg += vars_mcs[j];
            }
            out<<endl;
            out<<"avg="<<var_avg/qn<<endl;
            out<<"----------"<<endl;
            out.close();

            // TIPS: 将方差结果写入文件, SS_VL
            printf("%s: Writing variances to file, ss-vl ... \n", file.c_str());
            out.open("../output/ss_vl_variances_of_" + file, ios::app);
            out<<"k="<<k;
            var_avg = 0.0;
            for(int j = 0; j < qn; j++){
                if(j % 10 == 0) out<<endl;
                out<<vars_ss_vl[j]<<" ";
                var_avg += vars_ss_vl[j];
            }
            out<<endl;
            out<<"avg="<<var_avg/qn<<endl;
            out<<"----------"<<endl;
            out.close();

            // TIPS: 将方差结果写入文件, SS_SC
            printf("%s: Writing variances to file, ss-sc ... \n", file.c_str());
            out.open("../output/ss_sc_variances_of_" + file, ios::app);
            out<<"k="<<k;
            var_avg = 0.0;
            for(int j = 0; j < qn; j++){
                if(j % 10 == 0) out<<endl;
                out<<vars_ss_sc[j]<<" ";
                var_avg += vars_ss_sc[j];
            }
            out<<endl;
            out<<"avg="<<var_avg/qn<<endl;
            out<<"----------"<<endl;
            out.close();

            // TIPS: 将方差结果写入文件, LS
            printf("%s: Writing variances to file, ls ... \n", file.c_str());
            out.open("../output/ls_variances_of_" + file, ios::app);
            out<<"k="<<k;
            var_avg = 0.0;
            for(int j = 0; j < qn; j++){
                if(j % 10 == 0) out<<endl;
                out<<vars_ls[j]<<" ";
                var_avg += vars_ls[j];
            }
            out<<endl;
            out<<"avg="<<var_avg/qn<<endl;
            out<<"----------"<<endl;
            out.close();

//
//            // TIPS: 将方差结果写入文件, LSSA_VL
//            printf("%s: Writing variances to file, lssa-vl ... \n", file.c_str());
//            out.open("output/lssa_vl_variances_of_" + file, ios::app);
//            out<<"k="<<k;
//            var_avg = 0.0;
//            for(int j = 0; j < qn; j++){
//                if(j % 10 == 0) out<<endl;
//                out<<vars_lssa_vl[j]<<" ";
//                var_avg += vars_mcs[j];
//            }
//            out<<endl;
//            out<<"avg="<<var_avg/qn<<endl;
//            out<<"----------"<<endl;
//            out.close();
//
//            // TIPS: 将方差结果写入文件, LSSA_SC
//            printf("%s: Writing variances to file, lssa-sc ... \n", file.c_str());
//            out.open("output/lssa_sc_variances_of_" + file, ios::app);
//            out<<"k="<<k;
//            var_avg = 0.0;
//            for(int j = 0; j < qn; j++){
//                if(j % 10 == 0) out<<endl;
//                out<<vars_lssa_sc[j]<<" ";
//                var_avg += vars_mcs[j];
//            }
//            out<<endl;
//            out<<"avg="<<var_avg/qn<<endl;
//            out<<"----------"<<endl;
//            out.close();
        }
    }
}