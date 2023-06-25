#include <bits/stdc++.h>

using namespace std;

int main(void)
{
    string filename = "biomine.txt";

    ifstream in(filename);
    string temp;
    int x, y;
    double prob;
    int maxn = -1;
    double maxProb = -1, minProb = 2;
    int cnt = 0;

    while(getline(in, temp)){
        cnt++;
        stringstream ss(temp);
        ss>>x; ss>>y;
        if(x > maxn) maxn = x;
        if(y > maxn) maxn = y;
        ss>>prob;
        if(prob > maxProb) maxProb = prob;
        if(prob < minProb) minProb = prob;
    }

    printf("Number of vertices: %d\n", maxn);
    printf("Number of edges: %d\n", cnt);
    printf("Maximal probability: %.2f\n", maxProb);
    printf("Minimal probability: %.2f\n", minProb);

    in.close();
}