//
// Created by liuyue on 2021/3/17.
//

#include <bits/stdc++.h>

using namespace std;

/**
 * return n!
 * @param n
 * @return
 */
long long fact(int n)
{
    assert(n >= 0);
    if(n <= 1) return 1;
    long long ans = 1;
    for(int i = 2; i <= n; i++) ans *= i;
    return ans;
}


/**
 * Number of k-core in a (k+2)-clique
 * @param k
 * @return
 */
long long f(int k)
{
    long long m = (k+1)*(k+2)/2; // number of edges
    long long l = m / (2*k+1);
    long long ans = k+3; // 1 + k+2

    for(auto i = 0; i <= l; i++){
        long long temp = 1;
        for(auto j = 0; j <= i; j++){
            temp *= m - j * (2*k-2*j+3);
        }
        ans += temp / fact(i+1);
    }
    return ans;
}


int main()
{
    int k = 0;
    while(true){
        cin>>k;
        if(k < 2) break;
        cout<<f(k)<<endl;
    }
}

