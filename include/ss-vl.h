//
// Created by liuyue on 20/4/2021.
//

#ifndef SRC_SS_VL_H
#define SRC_SS_VL_H

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

vector<int> ss_vl(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  vector<int> &C, int q, int k, double theta,
                  vector<double> &Pr_v, int N=500, int r=50, double p0=0.5);

vector<int> ss_vl(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  vector<int> &C, int q, int k, double theta,
                  vector<double> &Pr_v, double k0);
vector<int> ss_vl_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                  vector<int> &C, int q, int k, double theta,
                  vector<double> &Pr_v, int N=500);


#endif //SRC_SS_VL_H
