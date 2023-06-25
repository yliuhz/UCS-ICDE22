//
// Created by liuyue on 20/4/2021.
//

#ifndef SRC_LS_H
#define SRC_LS_H

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

vector<int> ls(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
               vector<int> &C, int q, int k, double theta,
               vector<double> &Pr_v, int N=500);

vector<int> ls_f(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
               vector<int> &C, int q, int k, double theta,
               vector<double> &Pr_v, int N=500);

vector<int> ls_log(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                 vector<int> &C, int q, int k, double theta,
                 vector<double> &Pr_v, string &file, int N=500);

vector<int> ls_r(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
               vector<int> &C, int q, int k, double theta,
               vector<double> &Pr_v, int r);

vector<int> ls_fr(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                 vector<int> &C, int q, int k, double theta,
                 vector<double> &Pr_v, int N, int r);

#endif //SRC_LS_H
