//
// Created by liuyue on 19/4/2021.
//

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

int main(void)
{
    vector<string> files = {"email-Enron-p.txt",
                            "flickr.txt",
                            "dblp-graph-p-18-20.txt",
                            "biomine.txt"};

    base b;
    for(auto filename : files){
        cout<<filename<<endl;
        filename = "data/" + filename;
        b.readFilep(filename);
    }
}