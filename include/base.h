#ifndef BASE_H
#define BASE_H

#include <bits/stdc++.h>

using namespace std;

struct pairs
{
    int y;
    double prob;
};

class base
{
public:
    vector<vector<int>> readFile(string filename);
    vector<vector<pairs>> readFilep(string filename);
    vector<tuple<int,int,double>> readFileE(string filename);
    vector<pair<int,int>> readFileEd(string filename);

    vector<vector<pairs>> readFilep_from_exist(string filename);

    void writeToFile(string filename, vector<vector<int>> &G);
    void writeToFile(string filename, vector<int> &vs);
    void printg(vector<vector<int>> &G);
    void printg(vector<vector<pairs>> &G);
    void printv(vector<int> &vs);
    int comb(int n, int r);
    void printt(tuple<int,int,double> &t);
    void printg(vector<tuple<int,int,double>> &Edges);
    double avgProb(vector<tuple<int,int,double>> &Edges);

    double MSE(vector<double> &uc1, vector<double> &uc2);
    bool graphIsomorphism(vector<vector<pairs>> &G1, vector<vector<pairs>> &G2);
    vector<vector<int>> toDeter(vector<vector<pairs>> &G);

    double var(vector<vector<double>> &Prs);
    double cosine_dis(vector<double> &Pr1, vector<double> &Pr2);
    double norm2(vector<double> &Pr);

    int V, E;
};

// 并查集模板
class UnionFind {
public:
    vector<int> parent;
    vector<vector<int>> child;
    vector<int> size;
    int n;
    // 当前连通分量数目
    int setCount;
    
public:
    UnionFind(int _n): n(_n), setCount(_n), parent(_n), child(_n), size(_n, 1) {
        iota(parent.begin(), parent.end(), 0);
        for(int i = 0; i < _n; i++){
            child[i] = vector<int>{i};
        }
    }
    
    int findset(int x) {
        return parent[x] == x ? x : parent[x] = findset(parent[x]);
    }
    
    bool unite(int x, int y) {
        x = findset(x);
        y = findset(y);
        if (x == y) {
            return false;
        }
        if (size[x] < size[y]) {
            swap(x, y);
        }
        parent[y] = x;
        size[x] += size[y];
        --setCount;

        child[x].insert(child[x].end(), child[y].begin(), child[y].end());
        child[y].clear();
        return true;
    }
    
    bool connected(int x, int y) {
        x = findset(x);
        y = findset(y);
        return x == y;
    }

    /**
     *
     * @param x
     * @return 返回影响到的整个连通分量，删除x后只需要重建这个连通分量
     */
    vector<int> drop(int x)
    {
        int p = findset(x);
        vector<int> ret = child[p];
        for(auto v : ret) { // ASSERT: 循环条件中不能写成 (auto v : child[p])
            parent[v] = v;
            size[v] = 1;
            child[v] = vector<int>{v}; // TIPS: 不能改成{}空集，因为之后要重建
        }
        return ret;
    }

//    void rebuild(vector<int> &cc, vector<vector<pairs>> &G)
//    {
//        for(auto v : cc){
//            for(auto u : G[v]){
//                unite(u.y, v);
//            }
//        }
//    }

    vector<vector<int>> connected_components(int k)
    {
        vector<vector<int>> ret;
        for(int i = 0; i < n; i++){
            if(child[i].size() >= k) ret.emplace_back(child[i]);
        }
        return ret;
    }

};

// 并查集模板
// https://leetcode-cn.com/problems/remove-max-number-of-edges-to-keep-graph-fully-traversable/solution/bao-zheng-tu-ke-wan-quan-bian-li-by-leet-mtrw/
class UnionFind_ {
public:
    vector<int> parent;
    vector<int> size;
    int n;
    // 当前连通分量数目
    int setCount;

public:
    UnionFind_(int _n): n(_n), setCount(_n), parent(_n), size(_n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int findset(int x) {
        return parent[x] == x ? x : parent[x] = findset(parent[x]);
    }

    bool unite(int x, int y) {
        x = findset(x);
        y = findset(y);
        if (x == y) {
            return false;
        }
        if (size[x] < size[y]) {
            swap(x, y);
        }
        parent[y] = x;
        size[x] += size[y];
        --setCount;
        return true;
    }

    bool connected(int x, int y) {
        x = findset(x);
        y = findset(y);
        return x == y;
    }

    void drop_()
    {
        setCount = n;
        for(int i = 0; i < n; i++){
            size[i] = 1;
            parent[i] = i;
        }
    }

    vector<vector<int>> connected_components_(int k)
    {
        vector<vector<int>> ret;
        vector<bool> visit(n, false);
        for(int i = 0; i < n; i++){
            if(visit[i]) continue;
            vector<int> temp = {i};
            visit[i] = true;
            int p_i = findset(i);
            for(int j = i+1; j < n; j++){
                if(visit[j]) continue;
                int p_j = findset(j);
                if(p_i == p_j) {
                    visit[j] = true;
                    temp.emplace_back(j);
                }
            }
            if(temp.size() > k) ret.emplace_back(temp);
        }
        return ret;
    }
};


#endif