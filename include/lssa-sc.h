//
// Created by liuyue on 20/4/2021.
//

#ifndef SRC_LSSA_SC_H
#define SRC_LSSA_SC_H

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

vector<int> lssa_sc(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                   vector<int> &C, int q, int k, double theta,
                    vector<double> &Pr_v, int N=500, int r=50);

vector<int> lssa_sc_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                    vector<int> &C, int q, int k, double theta,
                    vector<double> &Pr_v, int N=500, int r=50);

vector<int> lssa_sc_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      vector<int> &C, int q, int k, double theta,
                      vector<double> &Pr_v, int N=500, int r=50);

vector<int> lssa_sc_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                        vector<int> &C, int q, int k, double theta,
                        vector<double> &Pr_v, int r);

vector<int> lssa_sc_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      vector<int> &C, int q, int k, double theta,
                      vector<double> &Pr_v, int N, int r);

#endif //SRC_LSSA_SC_H
