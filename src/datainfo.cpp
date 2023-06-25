#include <bits/stdc++.h>
#include "../include/base.h"

using namespace std;

/**
 *
 * @return the maximal degree, average degree, average probability on edges
 */

int main(void)
{
    base b;

    string folder = "../data/";

    vector<string> files = {
//            "dblp-graph-p.txt",
//            "dblp-graph-p-10-20.txt",
//            "dblp-graph-p-15-20.txt",
            "dblp-graph-p-18-20.txt",
//            "biomine.txt",
//            "OCnodeslinks.txt.txt",
//            "flickr.txt",
//            "email-Enron-p.txt"
    };

    string corePrefix = "_core.txt";

    for(auto filename : files){
        cout<<"----------"<<filename<<"-------------"<<endl;

        filename = folder+filename;
        ifstream in(filename);
        if(in.fail()){
            cout<<"FAILED: "<<filename<<endl;
            continue;
        }
        in.close();

        auto G = b.readFilep(filename);
        auto Edges = b.readFileE(filename);
        printf("V=%d, E=%d, avgProb=%.3f\n", b.V, b.E, b.avgProb(Edges));

        vector<int> deg(b.V);
        for(int i = 0; i < b.V; i++) deg[i] = G[i].size();

        printf("Maximal degree=%d, Average degree=%.2f\n",
               *max_element(deg.begin(), deg.end()),
               accumulate(deg.begin(), deg.end(), 0) * 1.0 / b.V);

        filename = filename+corePrefix;
        ifstream in2(filename);
        if(in2.fail()){
            cout<<"FAILED: "<<filename<<endl;
            continue;
        }
        string temp;
        int u, core_number, maxn=-1, sum=0;
        while(getline(in2, temp)){
            stringstream ss(temp);
            ss>>u; ss>>core_number;
            if(core_number > maxn) maxn = core_number;
            sum += core_number;
        }
        in2.close();

        printf("Maximal core number=%d, Average core number=%.2f\n",
               maxn,
               sum * 1.0 / b.V);
    }
}