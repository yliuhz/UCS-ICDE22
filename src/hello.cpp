#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"
#include "../include/ls.h"
#include "../include/rss-vl.h"
#include "../include/hello.h"
#include "../include/exact.h"

using namespace std;

void f(int a, int b)
{
    printf("%d,%d\n", a,b);
}

int main(void) {
    vector<vector<int>> vs = {{1,2,3},{4,5,6}}, vs2 = {{1,2,3},{4,5,0}};
    cout<<(int)(vs==vs2)<<endl;

    swap(vs[0][0], vs[0][1]);
    for(auto vss : vs){
        for(auto c : vss){
            cout<<c<<" ";
        }
        cout<<endl;
    }
}