#ifndef SRC_LOCALS_H
#define SRC_LOCALS_H

#include <bits/stdc++.h>
#include "../include/base.h"

using namespace std;

clock_t start, end;
#define START start=clock();
#define END(x) cout<<"Total time of "<<x<<": "<<(double)(clock()-start)/CLOCKS_PER_SEC<<" s"<<endl;


/** <<local search of communities in large graphs>> 
 * */

class locals
{
public:
    vector<int> global(int q, int k, vector<vector<int>> &G);
    vector<int> global(int q, int k, vector<vector<pairs>> &G);
    vector<int> global_e(int q, int k, vector<vector<int>> &G, vector<pair<int,int>> &Edges);
    vector<int> local(int q, int k, vector<vector<int>> &G, int V, int E);
    vector<int> local(int q, int k, vector<vector<pairs>> &G, int V, int E);
    vector<int> local_li(int q, int k, vector<vector<int>> &G, int V, int E);
    vector<vector<int>> extractSubgraph(vector<int> &C, vector<vector<int>> &G);
    vector<vector<pairs>> extractSubgraph(vector<int> &C, vector<vector<pairs>> &G);
    vector<vector<int>> extractSubgraph_rearrange(vector<int> &C, vector<vector<int>> &G);
    int minSubgraphDegree(vector<int> &C, vector<vector<int>> &G);
    vector<tuple<int,int,double>> table2Edges(vector<vector<pairs>> &G);
    vector<vector<pairs>> edges2Table(vector<tuple<int,int,double>> &Edges);
    vector<vector<pairs>> rearrange(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges, int &q);
    vector<vector<pairs>> rearrange(vector<vector<pairs>> &G, int &q);
    vector<vector<pairs>> rearrange(vector<vector<pairs>> &G, int &q, vector<int> &Cqk);
    
};

#endif