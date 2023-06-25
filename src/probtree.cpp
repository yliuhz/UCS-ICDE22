//
// Created by liuyue on 20/4/2021.
//

// ASSERT: 索引完最后也是采样算法，不是精确算法

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"

using namespace std;

struct bag
{
    vector<int> idx2vid;
    unordered_map<int,int> vid2idx;
    vector<vector<int>> g;
    int coveredVid;
    bool marked;

    bag *parent;

    bag(){
        g.resize(3, vector<int>(3, 0));
        marked = false;
        coveredVid = -1;
        parent = nullptr;
    }

    void insert(int v){
        idx2vid.emplace_back(v);
        vid2idx[v] = idx2vid.size()-1;
        if((int)g.size()-1 < vid2idx[v]) g.resize(vid2idx[v]+1);
    }

    void connect(int u, int v){
        int uidx = vid2idx[u], vidx = vid2idx[v];
        g[uidx][vidx] = g[vidx][uidx] = 1;
    }

    void setCovered(int v){
        coveredVid = v;
    }

    void setMarked(void){
        marked = true;
    }

    bool isMarked(void){
        return marked;
    }

};

void buildProbTree(vector<vector<pairs>> G, vector<tuple<int,int,double>> &Edges,
                   vector<vector<int>> dG, int w = 2)
{
    int n = G.size();
    int m = Edges.size();

    unordered_map<int,unordered_set<int>> degree2vid;
    vector<int> deg(n);
    for(int i = 0; i < n; i++){
        deg[i] = G[i].size();
        degree2vid[deg[i]].insert(i);
    }

    vector<vector<bool>> marked(n, vector<bool>(n,false));

    vector<bag*> S;

    unordered_set<int> uncovered;
    for(int i = 0; i < n; i++) uncovered.insert(i);

    for(int i = 1; i <= w; i++){
        while(!degree2vid[i].empty()){
            int v = i;
            bag* bg = new bag();
            bg->insert(v);
            vector<int> neib;
            for(auto u : dG[v]){
                neib.emplace_back(u);
                bg->insert(u);
                if(!marked[u][v]){
                    bg->connect(u, v);
                    marked[u][v] = marked[v][u] = true;
                }
                dG[u].erase(find(dG[u].begin(), dG[u].end(), v));
                degree2vid[deg[u]].erase(u);
                degree2vid[deg[u]-1].insert(u);
                --deg[u];
            }
            bg->setCovered(v);
            dG[v].clear();
            degree2vid[deg[v]].erase(v);
            deg[v] = 0;
            uncovered.erase(v);

            if(neib.size() == 2){ // TIPS：仅考虑w=2的情况
                int u1 = neib[0], u2 = neib[1];
                if(find(dG[u1].begin(), dG[u1].end(), u2) == dG[u1].end()){
                    dG[u1].emplace_back(u2);
                    dG[u2].emplace_back(u1);
                    degree2vid[deg[u1]].erase(u1);
                    degree2vid[deg[u1]+1].insert(u1);
                    ++deg[u1];

                    degree2vid[deg[u2]].erase(u2);
                    degree2vid[deg[u2]+1].insert(u2);
                    ++deg[u2];
                }
            }

            S.emplace_back(bg);
        }
    }

    if(!uncovered.empty()){
        bag *root = new bag();
        for(auto v : uncovered){
            root->insert(v);
        }

        for(auto e : Edges){
            int v1 = get<0>(e);
            int v2 = get<1>(e);
            if(uncovered.find(v1) != uncovered.end() &&
            uncovered.find(v2) != uncovered.end()){
                if(!marked[v1][v2]){
                    root->connect(v1, v2);
                    marked[v1][v2] = marked[v2][v1] = true;
                }
            }
        }

        S.emplace_back(root);
    }

    for(auto bg : S){
        bg->setMarked();
        vector<int> v_bg;
        for(auto vid : bg->idx2vid){
            if(vid != bg->coveredVid){
                v_bg.emplace_back(vid);
            }
        }
        for(auto bg2 : S){
            vector<int> v_bg2 = bg2->idx2vid;
            bool subset = true;
            for(auto v : v_bg){
                if(find(v_bg2.begin(), v_bg2.end(), v) == v_bg2.end()){
                    subset = false; break;
                }
            }
            if(subset){
                bg->parent = bg2;
                break;
            }
        }

        if(!bg->parent){
            bg->parent = S.back(); // bg->parent = root;
        }
    }


}

int main(void)
{

}