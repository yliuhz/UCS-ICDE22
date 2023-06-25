//
// Created by liuyue on 21/4/2021.
//

#include <bits/stdc++.h>

using namespace std;

int main(void)
{
    double maxp = 0.2;
    uniform_real_distribution<double> urd(0.0, maxp);

    int n = 100000;
    int m = n * 10;
    uniform_int_distribution<int> uid(0, n);
    random_device dev;
    default_random_engine re(dev());

    string outfile = "../data/synthetic_1.txt";
    ofstream out(outfile);
    for(int i = 0; i < m; i++){
        int u = uid(re), v = uid(re);
        double prob = urd(re);
        out<<u<<" "<<v<<" "<<prob<<endl;
    }
    out.close();
}