//
// Created by liuyue on 20/4/2021.
//

#ifndef SRC_MCS_WITH_PRUNE_H
#define SRC_MCS_WITH_PRUNE_H

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

vector<int> mcs_with_prune(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                           vector<int> &C, int q, int k, double theta,
                           vector<double> &Pr_v, int N=500);

vector<int> mcs_with_prune_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                           vector<int> &C, int q, int k, double theta,
                           vector<double> &Pr_v, int N=500);

vector<int> mcs_with_prune_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                             vector<int> &C, int q, int k, double theta,
                             vector<double> &Pr_v, string &file, int N=500);

vector<int> mcs_with_prune_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                               vector<int> &C, int q, int k, double theta,
                               vector<double> &Pr_v, int r);

vector<int> mcs_with_prune_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                             vector<int> &C, int q, int k, double theta,
                             vector<double> &Pr_v, int N, int r);

vector<int> baseline(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                           int q, int k, double theta,
                           vector<double> &Pr_v, int N=500);

#endif //SRC_MCS_WITH_PRUNE_H
