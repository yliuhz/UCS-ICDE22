
#include "../include/locals.h"

vector<int> locals::global(int q, int k, vector<vector<int>> &G)
{
    if(G[q].size() < k) return vector<int>{};
    int n = G.size();

    // 第一步：不断删除度<k的顶点
    vector<int> deg(n);
    for(int i = 0; i < n; i++) deg[i] = G[i].size();
    vector<bool> inans(n, true);
    while(1){
        vector<int> temp;
        for(int i = 0; i < n; i++){     
            if(inans[i] && deg[i] < k) {
                if(i == q) return vector<int>{};
                temp.emplace_back(i);
            }
        }

        if(temp.empty()) break;
        for(auto c : temp){
            inans[c] = false;
            for(auto w : G[c]){ // w = c's neighbor
                deg[w]--;
            }
        }
    }
    
    // 第二步：返回包含q的连通分量
    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(inans[i]) ans.emplace_back(i);
    }
    

    UnionFind uf(n);
    for(auto c : ans){
        for(auto w : G[c]){
            if(inans[w]) uf.unite(c, w);
        }
    }
    
    // 返回结果
    vector<int> res;
    for(auto c : ans){
        if(uf.connected(q, c)) res.emplace_back(c);
    }
        
    return res;
}

vector<int> locals::global(int q, int k, vector<vector<pairs>> &G) {
    int n = G.size();

    // 第一步：不断删除度<k的顶点
    vector<int> deg(n);
    for(int i = 0; i < n; i++) deg[i] = G[i].size();
    vector<bool> inans(n, true);
    while(1){
        vector<int> temp;
        for(int i = 0; i < n; i++){
            if(inans[i] && deg[i] < k) {
                if(i == q) return vector<int>{};
                temp.emplace_back(i);
            }
        }

        if(temp.empty()) break;
        for(auto c : temp){
            inans[c] = false;
            for(auto w : G[c]){ // w = c's neighbor
                deg[w.y]--;
            }
        }
    }

    // 第二步：返回包含q的连通分量
    vector<int> ans;
    for(int i = 0; i < n; i++){
        if(inans[i]) ans.emplace_back(i);
    }


    UnionFind uf(n);
    for(auto c : ans){
        for(auto w : G[c]){
            if(inans[w.y]) uf.unite(c, w.y);
        }
    }

    // 返回结果
    vector<int> res;
    for(auto c : ans){
        if(uf.connected(q, c)) res.emplace_back(c);
    }

    return res;
}

vector<int> locals::global_e(int q, int k, vector<vector<int>> &G, vector<pair<int, int>> &Edges) {
    int n = G.size();

    vector<int> deg(n);

    for(auto e : Edges){
        deg[e.first] = G[e.first].size();
        deg[e.second] = G[e.second].size();
    }

    while(1){
        vector<pair<int,int>> temp;
        for(auto e : Edges){
            if(deg[e.first] < k || deg[e.second] < k){
                temp.emplace_back(e);
            }
        }

        if(temp.empty()) break;
        for(auto e : temp){
            Edges.erase(find(Edges.begin(), Edges.end(), e));
            deg[e.first]--; deg[e.second]--;
            if(deg[q] < k) return vector<int>{};
        }
    }

    UnionFind uf(n);
    for(auto e : Edges){
        uf.unite(e.first, e.second);
    }

    unordered_set<int> ans;
    for(auto e : Edges){
        if(uf.connected(e.first, q)) ans.insert(e.first);
        if(uf.connected(e.second, q)) ans.insert(e.second);
    }

    vector<int> ret; ret.assign(ans.begin(), ans.end());
    return ret;
}

/**
 * TODO: 
 * */
//vector<int> locals::local(int q, int k, vector<vector<int>> &G, int V, int E)
//{
//    // 第一步：使用k的上界检验k是否过大
//    // <<local search of communities in large graphs>>
//    if(k > min((int)G[q].size(), (int)(1+sqrt(9+8*(E-V)))/2)) return vector<int>{};
//
//    // 第二步：生成候选集
//    queue<int> Q; Q.push(q);
//    vector<int> C;
//    vector<bool> visit(V);
//    visit[q] = true;
//    vector<bool> inc(V);
//    inc[q] = true;
//    vector<int> deg(V, V);
//    deg[q] = 0;
//    auto cmp = [deg](int x, int y){
//        return deg[x] < deg[y]; // 小根堆
//    };
//    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
//    while(!Q.empty()){
//        int v = Q.front();
//        Q.pop();
//        C.emplace_back(v);
//        inc[v] = true;
//        deg[v] = 0;
//        // TODO: 快速求G[C]中的最小度
//        // if(minSubgraphDegree(C,G) >= k) return C;
//        for(auto w : G[v]){
//            if(!visit[w] && G[w].size() >= k) {
//                Q.push(w);
//                visit[w] = true;
//            }
//            if(inc[w]){
//                deg[w]++; deg[v]++;
//            }
//        }
//        pq.push(v);
//        if(deg[pq.top()] >= k) return C; // TODO: Local_ex_2.txt测试不通过
//        // printf("Q size= %d\n", (int)Q.size());
//    }
//
//    // 第三步：若第二步未返回结果，则执行global全局搜索
//    auto H = extractSubgraph(C, G);
//    C = global(q, k, H);
//
//    // 返回结果
//    return C;
//}

vector<int> locals::local(int q, int k, vector<vector<int>> &G, int V, int E)
{
    if(k > min((int)G[q].size(), (int)(1+sqrt(9+8*(E-V)))/2)) return vector<int>{};

    vector<int> C;
    unordered_map<int,int> mp; // vertex -- degree
    unordered_set<int> visit; // visited in Q
    queue<int> Q;
    Q.push(q);
    visit.insert(q);

    while(!Q.empty()){
        int v = Q.front();
        Q.pop();
        C.emplace_back(v);
        mp[v] = 0;
        for(auto w : G[v]){
            if(mp.count(w)) {
                mp[w]++; mp[v]++;
            }
        }

        auto mindeg = *min_element(mp.begin(), mp.end(), [](auto &p1, auto &p2){
            return p1.second < p2.second;
        });

        if(mindeg.second >= k) return C;

        for(auto w : G[v]){
            if(G[w].size() >= k && !visit.count(w)){
                Q.push(w); visit.insert(w);
            }
        }
    }

    auto H = extractSubgraph(C, G);
//    auto H = extractSubgraph_rearrange(C, G);

    C = global(q, k, H);

    return C;
}

vector<int> locals::local(int q, int k, vector<vector<pairs>> &G, int V, int E) {
    if(k > min((int)G[q].size(), (int)(1+sqrt(9+8*(E-V)))/2)) return vector<int>{};

    vector<int> C;
    unordered_map<int,int> mp; // vertex -- degree
    unordered_set<int> visit; // visited in Q
    queue<int> Q;
    Q.push(q);
    visit.insert(q);

    while(!Q.empty()){
        int v = Q.front();
        Q.pop();
        C.emplace_back(v);
        mp[v] = 0;
        for(auto w : G[v]){
            if(mp.count(w.y)) {
                mp[w.y]++; mp[v]++;
            }
        }

        auto mindeg = *min_element(mp.begin(), mp.end(), [](auto &p1, auto &p2){
            return p1.second < p2.second;
        });

        if(mindeg.second >= k) return C;

        for(auto w : G[v]){
            if(G[w.y].size() >= k && !visit.count(w.y)){
                Q.push(w.y); visit.insert(w.y);
            }
        }
    }

    auto H = extractSubgraph(C, G);
//    auto H = extractSubgraph_rearrange(C, G);

    C = global(q, k, H);

    return C;
}

vector<int> locals::local_li(int q, int k, vector<vector<int>> &G, int V, int E)
{
    if(k > min((int)G[q].size(), (int)(1+sqrt(9+8*(E-V)))/2)) return vector<int>{};

    int n = G.size();

    vector<int> C; // 候选集
    map<int,unordered_set<int>> subgraph_degree; // G[C]中度->顶点
    vector<int> deg(n, 0); // G[C]中的度
    map<int,unordered_set<int>> priority; // 加入到Q中的优先级
    vector<int> f(n,0);

    for(int i = 0; i < n; i++) priority[0].insert(i);

    queue<int> Q;
    Q.push(q);
    priority[0].erase(q);
    f[q] = -1;

    while(!Q.empty()){
        int v = Q.front();
        Q.pop();
        C.emplace_back(v);
        int v_deg = 0;
        for(auto u : G[v]){
            if(G[u].size() < k) break;
            if(f[u] >= 0){
                priority[f[u]].erase(u);
                if(priority[f[u]].empty()) priority.erase(f[u]);
                priority[f[u]+1].insert(u);
                ++f[u];
            }

//            if(C.find(u) != C.end())
            else {
                ++v_deg;
                subgraph_degree[deg[u]].erase(u);
                if(subgraph_degree[deg[u]].empty()) subgraph_degree.erase(deg[u]);
                subgraph_degree[deg[u]+1].insert(u);
                ++deg[u];
            }
        }

        deg[v] = v_deg;
        subgraph_degree[deg[v]].insert(v);

        if(subgraph_degree.begin()->first >= k) return C;

        if(priority.empty()) break;

        int max_priority_v = *(priority.rbegin()->second.begin());
        Q.push(max_priority_v);
        f[max_priority_v] = -1;
        priority.rbegin()->second.erase(max_priority_v);
        if(priority.rbegin()->second.empty()){
            int max_priority = priority.rbegin()->first;
            priority.erase(max_priority);
        }
    }

    auto H = extractSubgraph(C, G);

    C = global(q, k, H);

    return C;
}

vector<vector<int>> locals::extractSubgraph(vector<int> &C, vector<vector<int>> &G)
{
    // 从大图G中抽出子图G[C]
    int n = G.size();
    vector<vector<int>> H(n);
    unordered_set<int> ss;
    for(auto c : C) ss.insert(c);
    for(auto c : C){
        for(auto w : G[c]){
            if(ss.count(w)) {
                H[c].emplace_back(w);
//                H[w].emplace_back(c); // ASSERT: 不能写这一行，会重复加点
            }
        }
    }
    return H;
}

vector<vector<pairs>> locals::extractSubgraph(vector<int> &C, vector<vector<pairs>> &G) {
    // TIPS: 时间复杂度O(m'+n')
    int n = G.size();
    vector<vector<pairs>> H(n);
    unordered_set<int> ss;
    for(auto c : C) ss.insert(c);
    for(auto c : C){
        for(auto w : G[c]){
            if(ss.count(w.y)) {
                H[c].emplace_back(w);
//                H[w].emplace_back(c); // ASSERT: 不能写这一行，会重复加点
            }
        }
    }
    return H;
}

vector<vector<int>> locals::extractSubgraph_rearrange(vector<int> &C, vector<vector<int>> &G) {
    int n = C.size();
    unordered_map<int,int> mp;
    for(int i = 0; i < n; i++){
        mp[C[i]] = i;
    }
    vector<vector<int>> H(n);
    for(auto c : C){
        for(auto w : G[c]){
            if(mp.count(w)){
                H[mp[c]].emplace_back(mp[w]);
            }
        }
    }

    return H;
}

int locals::minSubgraphDegree(vector<int> &C, vector<vector<int>> &G)
{
    auto H = extractSubgraph(C, G);
    vector<int> deg;
    for(auto vs : H) {
        if(vs.size() > 0) deg.emplace_back(vs.size());
    }
    if(deg.empty()) return 0;
    return *min_element(deg.begin(), deg.end());
}

void deleteOneNode(int x, vector<vector<int>> &G)
{
    assert(x < G.size());
    for(auto w : G[x]){
        G[w].erase(find(G[w].begin(), G[w].end(), x));
    }
    G[x].clear();
}

vector<tuple<int,int,double>> locals::table2Edges(vector<vector<pairs>> &G) {
    vector<tuple<int,int,double>> ans;

    for(int i = 0; i < G.size(); i++){
        for(auto w : G[i]){
            int j = w.y;
            double prob = w.prob;
            if(i < j){
                ans.emplace_back(i, j, prob);
            }
        }
    }
    return ans;
}

vector<vector<pairs>> locals::edges2Table(vector<tuple<int,int,double>> &Edges)
{
    vector<vector<pairs>> H;
    for(auto e : Edges){
        int u = get<0>(e);
        int v = get<1>(e);
        double prob = get<2>(e);
        if((int)H.size()-1 < max(u,v)) H.resize(max(u,v)+1);
        H[u].emplace_back(pairs{v, prob});
        H[v].emplace_back(pairs{u, prob});
    }
    return H;
}

vector<vector<pairs>> locals::rearrange(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                                        int &q) {
    unordered_map<int,int> mp; // 转换id
    int cnt = 0;
    vector<vector<pairs>> ans;
    for(int i = 0; i < G.size(); i++){
        if(G[i].empty()) continue;
        if(!mp.count(i)) mp.emplace(i, cnt++);
        if((int)ans.size()-1 < mp[i]) ans.resize(mp[i]+1);
        for(auto w : G[i]){
            int j = w.y;
            if(i > j) continue;

            double prob = w.prob;
            if(!mp.count(j)) mp.emplace(j, cnt++);
            if((int)ans.size()-1 < mp[j]) ans.resize(mp[j]+1);

            // i < j
            ans[mp[i]].emplace_back(pairs{mp[j], prob});
            ans[mp[j]].emplace_back(pairs{mp[i], prob});
        }
    }

    vector<tuple<int,int,double>> Edges_2;
    for(auto e : Edges){
        int u = get<0>(e), v = get<1>(e);
        if(mp.count(u) && mp.count(v)) {
            double prob = get<2>(e);
            Edges_2.emplace_back(mp[u], mp[v], prob);
        }
    }

    q = mp[q];
    Edges = Edges_2;
    return ans;
}

vector<vector<pairs>> locals::rearrange(vector<vector<pairs>> &G, int &q) {
    unordered_map<int,int> mp; // 转换id
    int cnt = 0;
    vector<vector<pairs>> ans;
    for(int i = 0; i < G.size(); i++){
        if(G[i].empty()) continue;
        if(!mp.count(i)) mp.emplace(i, cnt++);
        if((int)ans.size()-1 < mp[i]) ans.resize(mp[i]+1);
        for(auto w : G[i]){
            int j = w.y;
            if(i > j) continue;

            double prob = w.prob;
            if(!mp.count(j)) mp.emplace(j, cnt++);
            if((int)ans.size()-1 < mp[j]) ans.resize(mp[j]+1);

            // i < j
            ans[mp[i]].emplace_back(pairs{mp[j], prob});
            ans[mp[j]].emplace_back(pairs{mp[i], prob});
        }
    }

    q = mp[q];
    return ans;
}

vector<vector<pairs>> locals::rearrange(vector<vector<pairs>> &G, int &q, vector<int> &Cqk) {
    unordered_map<int,int> mp; // 转换id
    int cnt = 0;
    vector<vector<pairs>> ans;
    for(int i = 0; i < G.size(); i++){
        if(G[i].empty()) continue;
        if(!mp.count(i)) mp.emplace(i, cnt++);
        if((int)ans.size()-1 < mp[i]) ans.resize(mp[i]+1);
        for(auto w : G[i]){
            int j = w.y;
            if(i > j) continue;

            double prob = w.prob;
            if(!mp.count(j)) mp.emplace(j, cnt++);
            if((int)ans.size()-1 < mp[j]) ans.resize(mp[j]+1);

            // i < j
            ans[mp[i]].emplace_back(pairs{mp[j], prob});
            ans[mp[j]].emplace_back(pairs{mp[i], prob});
        }
    }

    q = mp[q];
    for(auto &v : Cqk) v = mp[v];
    return ans;
}

int main(void)
{
    string filename = "../data/OCnodeslinks.txt.txt";
    base b;
    auto G = b.readFile(filename);
    auto Edges = b.readFileEd(filename);
    // b.printg(G);
//    b.writeToFile(filename+".txt", G);
    printf("V=%d, E=%d\n", b.V, b.E);
    printf("Edges size= %d\n", Edges.size());

    locals lo;
    vector<int> ans;

    int q = 1056;
    int k = 10;

    int n = 100;

    START
    ans = lo.global(q, k, G);
    END("GLOBAL")
    sort(ans.begin(), ans.end());
    printf("C size= %d\n", (int)ans.size());
//    b.printv(ans);


//    START
//    ans = lo.global_e(q, k, G, Edges);
//    END("GLOBAL_e")
//    printf("C size= %d\n", (int)ans.size());

//    START
//    ans = lo.local(q, k, G, b.V, b.E);
//    END("LOCAL")
//    sort(ans.begin(), ans.end());
//    printf("C size= %d\n", (int)ans.size());
////    b.printv(ans);


    START
    ans = lo.local_li(q, k, G, b.V, b.E);
    END("LOCAL_li")
    sort(ans.begin(), ans.end());
    printf("C size= %d\n", (int)ans.size());
//    b.printv(ans);

//    auto pG = b.readFilep(filename);
//    auto pE = b.readFileE(filename);
//
//    START
//    ans = lo.global(q, k, G);
//    END("GLOBAL-p")
//    sort(ans.begin(), ans.end());
//    printf("C size= %d\n", (int)ans.size());
//
//    START
//    ans = lo.local(q, k, G, b.V, b.E);
//    END("LOCAL-p")
//    sort(ans.begin(), ans.end());
//    printf("C size= %d\n", (int)ans.size());
}
