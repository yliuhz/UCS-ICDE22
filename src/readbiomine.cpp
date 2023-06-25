#include <bits/stdc++.h>

using namespace std;

int main(void)
{
    string filename = "../data/biomine_3_oct_2018.bmg";
    string outfile = "../data/biomine.txt";

    ifstream in(filename);
    ofstream out(outfile);
    string temp, outstr;

    int n = -1;
    unordered_map<string,int> mp;
    string sx, sy;
    double prob;
    int x, y;

    int i = 0;
    while(getline(in, temp)){
        i++;
        if(i % 10000 == 0){
            printf("%d\n", i);
        }
        if(temp[0] == '#') continue;
        stringstream ss(temp);
        ss>>sx; ss>>sy;
        if(!mp.count(sx)) mp[sx] = x = ++n;
        else x = mp[sx];
        if(!mp.count(sy)) mp[sy] = y = ++n;
        else y = mp[sy];
        ss>>sx; // 跳过边类型
        ss>>prob;
        outstr = to_string(x) + " " + to_string(y) + " " + to_string(prob) + "\n";
        out<<outstr;
    }

    in.close();
    out.close();
}