//
// Created by liuyue on 21/4/2021.
// TIPS: 测时间，关于k
// 1520:0.8
//

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"
#include "../include/probabilistic_kcore_computation.h"
#include "../include/exact.h"

using namespace std;

vector<vector<int>> qs = {
        { // sync_10_0
                1,3,5,6,7
        },
        { // sync_10_1
                1,2,3,7,8
        },
        { // sync_10_2
                1,3,4,5,6
        },
        { // sync_10_3
                0,1,3,4,8
        },
        { // sync_10_4
                1,2,5,6,7
        },
        { // sync_20_0
                0,1,4,5,8
        },
        { // sync_20_1
                1,2,3,4,5
        },
        { // sync_20_2
                0,1,3,5,10
        },
        { // sync_20_3
                0,1,2,3,4
        },
        { // sync_20_4
                0,1,3,8,12
        },
        { // sync_30_0
                0,1,8,9,10
        },
        { // sync_30_1
                0,4,7,9,11
        },
        { // sync_30_2
                0,1,6,7,8
        },
        { // sync_30_3
                1,2,4,5,8
        },
        { // sync_30_4
                1,4,9,11,12
        }
};

int main(void)
{
    vector<string> files = {
            "sync_10_0.txt",
            "sync_10_1.txt",
            "sync_10_2.txt",
            "sync_10_3.txt",
            "sync_10_4.txt",
            "sync_20_0.txt",
            "sync_20_1.txt",
            "sync_20_2.txt",
            "sync_20_3.txt",
            "sync_20_4.txt",
            "sync_30_0.txt",
            "sync_30_1.txt",
            "sync_30_2.txt",
            "sync_30_3.txt",
            "sync_30_4.txt",
    };

    int k = 2;
    double theta = 0.8;
    base b;
    locals lo;

    for(int i = 0; i < files.size(); i++){
        string file = files[i];
        string filename = "data/" + files[i];
        string outfile = "output/"+file+"_out.txt";
        ofstream out(outfile);

        auto G = b.readFilep(filename);
        auto Edges = b.readFileE(filename);
        int n = b.V;
        int m = b.E;
        cout<<"V="<<b.V<<",E="<<b.E<<endl;

        auto qs_of_this_file = qs[i];

        for(int j = 0; j < qs_of_this_file.size(); j++) {
            cout<<file<<": "<<j+1<<"/"<<qs_of_this_file.size()<<" q"<<endl;
            int q = qs_of_this_file[j];

            auto Cqk = lo.global(q, k, G);
            G = lo.extractSubgraph(Cqk, G);
            Edges = lo.table2Edges(G);

            cout << "Node size=" << Cqk.size() << endl;
            cout << endl;
            cout << "Edges size=" << Edges.size() << endl;

            vector<double> Pr_v;
            vector<int> vs;
            clock_t start, end;
            double time;

            start = clock();
            vs = EXACT_ENUM(G, Edges, q, k, theta, Pr_v);
            end = clock();
            time = (double) (end - start) / CLOCKS_PER_SEC;
            cout << "Total time of EXACT_ENUM: " << time << endl;
            out << time << " ";
        }

        out.close();
    }
}