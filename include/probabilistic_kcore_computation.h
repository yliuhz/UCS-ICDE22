//
// Created by liuyue on 20/4/2021.
//

#ifndef SRC_PROBABILISTIC_KCORE_COMPUTATION_H
#define SRC_PROBABILISTIC_KCORE_COMPUTATION_H

#include <bits/stdc++.h>
#include "../include/core_decomposition.h"
#include "../include/locals.h"

using namespace std;

class pkcore
{
public:
    pkcore() = default;
    pkcore(int v, int e) : V(v), E(e) {}

    int samplesNeeded(double epsilon, double delta);
    vector<int> basicSampling(vector<tuple<int,int,double>> &Edges, int k, double theta, double epsilon, double delta);
    vector<vector<int>> sample(vector<tuple<int,int,double>> &Edges, default_random_engine &re);
    vector<vector<int>> sample(vector<vector<pairs>> &G, default_random_engine &re);
    double degEqualK(vector<vector<pairs>> &G, int v, int i);
    double degGeK(vector<vector<pairs>> &G, int v, int i);
    vector<double> upperb(vector<vector<pairs>> &G, int k, double delta);

    void kcoreMembershipCheck(vector<vector<int>> &G, vector<int> &T, vector<int> &state,
                              vector<vector<pairs>> &UG, int k);

    vector<int> advancedSampling(vector<tuple<int,int,double>> &Edges, vector<vector<pairs>> &G, int k, double theta,
                                 double epsilon, double delta);

    vector<vector<int>> toDeter(vector<vector<pairs>> &G);
    vector<vector<pairs>> extractSubgraph(vector<vector<pairs>> &G, vector<int> &vs);

    double avg(vector<int> &vs);

    int V, E;
};

enum{
    KEEP,
    DEL,
    UNCERTAIN
};

#endif //SRC_PROBABILISTIC_KCORE_COMPUTATION_H
