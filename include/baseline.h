//
// Created by liuyue on 20/4/2021.
//

#ifndef SRC_BASELINE_H
#define SRC_BASELINE_H

class baseline
{
public:
    vector<vector<int>> sampleG(vector<tuple<int,int,double>> &Edges, int n, default_random_engine &re);
    vector<int> basef(vector<tuple<int,int,double>> &Edges, int n, int q, int k, double theta);
    vector<tuple<int,int,double>> toDeter(vector<tuple<int,int,double>> &Edges, int l);
    vector<tuple<int,int,double>> seg(vector<tuple<int,int,double>> &Edges, int l);
};

#endif //SRC_BASELINE_H
