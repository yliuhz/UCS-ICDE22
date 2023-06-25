#include <bits/stdc++.h>
#include "../include/base.h"

using namespace std;

class CSP
{
public:
    double computeDeg(int v, int k, vector<vector<pairs>> &G); // Return Pr(deg(v)>=k)
    vector<int> func(int q, int k, double theta, vector<vector<pairs>> &G);
    void extract(int v, int k, double theta, vector<int> &Q, vector<int> &C, vector<vector<pairs>> &G);
};

/*
** My new framework test
** Input: q, k, theta and uncertain graph G
** Return: A vertex set C, satisfying for every v $\in$ C, Pr(v in q's community) >= theta
** TODO: You are a superman!
*/

vector<int>
CSP::func(int q, int k, double theta, vector<vector<pairs>> &G)
{
    assert(q >= 0 && q < G.size());
    vector<int> C = {q};
    vector<int> Base, Added, Cand;
    vector<int> Old, Q;
    vector<int> cand;
    if(computeDeg(q, k, G) < theta) return vector<int>{};
    // Base.emplace_back(q);
    int neighborCnt = 0;
    for(auto w : G[q]){
        if(computeDeg(w.y, k, G) >= theta){
            Base.emplace_back(w.y);
            neighborCnt = neighborCnt+1;
        }
    }
    if(neighborCnt < k) return vector<int>{};
    
    unordered_map<int,int> mp; // 统计与社区内顶点的连边个数
    unordered_map<int,bool> visit;
    visit[q] = true;
    for(auto w : Base){
        mp[w]++;
        visit[w] = true;
    }

    Old = Base;
    C = Base;
    while(1){
        for(auto v : Old){
            // 检查v的邻居
            
        }
    }
}



void
CSP::extract(int v, int k, double theta, vector<int> &Q, vector<int> &C, vector<vector<pairs>> &G)
{
    for(auto s : G[v]){
        // 检查v的邻居s

    }
}

/*
** From <<Core decomposition of uncertain graphs>>, SIGKDD 14
** Time complexity: O(Kdeg(v)) (时间复杂度：O(K*v的度))
** Return: Pr(deg(v)>=k) (返回概率图中v的度大于等于k的概率)
*/

double
CSP::computeDeg(int v, int k, vector<vector<pairs>> &G)
{
    if(k <= 0) return 1.0;
    assert(v >= 0 && v < G.size());
    int n = G[v].size(); // deg(v)
    double ans = 0.0;
    vector<vector<double>> dp(n, vector<double>(k)); // dp[h][j] = 考虑到v的第h(0-n-1)个邻居时v的度=j的概率
    dp[0][0] = 1 - G[v][0].prob; dp[0][1] = G[v][0].prob;
    for(int j = 2; j < k; j++) dp[0][j] = 0;
    for(int h = 1; h < n; h++){
        dp[h][0] = dp[h-1][0] * (1 - G[v][h].prob);
    }
    for(int h = 1; h < n; h++){
        for(int j = 1; j < k; j++){
            dp[h][j] = G[v][h].prob * dp[h-1][j-1] + 
                        (1 - G[v][h].prob) * dp[h-1][j];
        }
    }
    for(int j = 0; j < k; j++) ans += dp[n-1][j];
    return 1-ans;
}

int main(void)
{
    string filename = "input.txt";
    base b;
    auto G = b.readFilep(filename);
    b.printg(G);

    CSP csp;
    int v = 6;
    int k = 0;
    printf("Pr(deg(%d)>=%d)= %f\n", v, k, csp.computeDeg(v, k, G));
}