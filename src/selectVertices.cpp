//
// Created by liuyue on 22/4/2021.
//

#include <bits/stdc++.h>

using namespace std;

int main(void)
{
//    vector<int> ks = {3, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 120,
//                      43, 510, 448, 1349, 400};
    vector<int> ks = {2,3,4,5,6,7,8,9,10,40};
    vector<vector<int>> qs(ks.size());
    int qn = 50;

    vector<string> files = {
            "email-Enron-p.txt",
            "flickr.txt",
            "dblp-graph-p-18-20.txt",
            "biomine.txt"
    };

    for(auto filename : files){
        filename = "../data/"+filename+"_core.txt";
        string outfile = filename + "_sel_vs.txt";

        ifstream in(filename);
        ofstream out(outfile);

        string temp;
        while(getline(in, temp)){
            stringstream ss(temp);
            int v, core_number;
            ss>>v; ss>>core_number;

            for(int i = 0; i < ks.size(); i++){
                int k = ks[i];

                if(core_number == k){
                    qs[i].emplace_back(v); // TIPS: qs[i]=core number为ks[i]的顶点
                    break;
                }
            }
        }

        for(int i = 0; i < ks.size(); i++){
            out<<"k="<<ks[i]<<", size="<<qs[i].size();
            for(int j = 0; j < qs[i].size(); j++){
                if(j % 10 == 0) out<<endl;
                out<<qs[i][j]<<", ";
            }
            out<<endl;
        }

        in.close();
        out.close();

        qs.clear();
        qs.resize(ks.size());
    }
}