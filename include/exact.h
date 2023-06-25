//
// Created by liuyue on 14/5/2021.
//

#ifndef SRC_EXACT_H
#define SRC_EXACT_H

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

void dfs(int idx, int r, int n, int m, vector<vector<int>> &ans, vector<int> &temp);
vector<vector<int>> edge2Vector(vector<int> sel, vector<tuple<int,int,double>> &Edges, int n);
double prob_of_possible_w(vector<int> sel, vector<tuple<int,int,double>> &Edges, int n);
vector<int> EXACT_POS(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      vector<int> &C, int q, int k, double theta,
                      vector<double> &Pr_v);
vector<int> EXACT_POS(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      int q, int k, double theta,
                      vector<double> &Pr_v);
//void exact_enum_core(vector<tuple<int,int,double>> &Edges, int q, int k,
//                     vector<double> &Pr_v);
void exact_enum_core(vector<tuple<int,int,double>> &Edges, int q, int k);
vector<int> EXACT_ENUM(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                       int q, int k, double theta, vector<double> &Pr_v);
void combine(int n, int k, vector<vector<int>> &ans, vector<int> &temp);

int cnt;
vector<set<tuple<int,int,double>> > visited;

#endif //SRC_EXACT_H
