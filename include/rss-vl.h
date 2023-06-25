//
// Created by liuyue on 21/4/2021.
//

#ifndef SRC_RSS_VL_H
#define SRC_RSS_VL_H

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

vector<int> rss_vl(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                   int q, int k, double theta,
                   vector<double> &Pr_v);

#endif //SRC_RSS_VL_H
