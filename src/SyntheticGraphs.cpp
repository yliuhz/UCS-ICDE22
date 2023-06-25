//
// Created by liuyue on 18/6/2021.
//

#include <bits/stdc++.h>

using namespace std;

int main(void)
{
    vector<int> ns = {
            10,
            20,
            30,
            40,
            50,
    };

    vector<int> es = {
            10,
            20,
            30,
            40,
            50,
    };

    for(int i = 0; i < ns.size(); i++){
        for(int j = 0; j < 10; j++){
            string outfile = "../data/sync_"+to_string(ns[i])+"_"+to_string(j)+".txt";
            ofstream out(outfile);
            int nn = ns[i], ee = es[i];

            int ecnt = 0;
            uniform_int_distribution<int> uid {0,nn-1};
            random_device dev;
            default_random_engine re(dev());

            uniform_real_distribution<double> urd(0.0,1.0);

            set<pair<int,int>> ss;

            while(ecnt < ee){
                int u = uid(re), v = uid(re);
                if(u != v && ss.find(pair{u,v}) == ss.end()){
                    double prob = urd(re);
                    ecnt++;
                    out<<u<<" "<<v<<" "<<prob<<endl;
                    ss.insert(pair{u,v});
                    ss.insert(pair{v,u});
                }
            }

            out.close();
        }
    }
}