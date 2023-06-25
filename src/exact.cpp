#include "../include/exact.h"

// @Return: 0-n-1中取m个数字的所有组合，结果保存在ans中
void dfs(int idx, int r, int n, int m, vector<vector<int>> &ans, vector<int> &temp)
{
    if(idx >= n){
        if(r == m){
            ans.emplace_back(temp);
        }
        return;
    }

    temp.emplace_back(idx);
    dfs(idx+1, r+1, n, m, ans, temp);
    temp.pop_back();
    dfs(idx+1, r-1, n, m, ans, temp);
}

// https://leetcode-cn.com/problems/combinations/solution/zu-he-by-leetcode-solution/
void combine(int n, int k, vector<vector<int>> &ans, vector<int> &temp) {
    // 初始化
    // 将 temp 中 [0, k - 1] 每个位置 i 设置为 i + 1，即 [0, k - 1] 存 [1, k]
    // 末尾加一位 n + 1 作为哨兵
    for (int i = 0; i < k; ++i) {
        temp.push_back(i);
    }
    temp.push_back(n);

    int j = 0;
    while (j < k) {
        ans.emplace_back(temp.begin(), temp.begin() + k);
        j = 0;
        // 寻找第一个 temp[j] + 1 != temp[j + 1] 的位置 t
        // 我们需要把 [0, t - 1] 区间内的每个位置重置成 [1, t]
        while (j < k && temp[j] + 1 == temp[j + 1]) {
            temp[j] = j;
            ++j;
        }
        // j 是第一个 temp[j] + 1 != temp[j + 1] 的位置
        ++temp[j];
    }
}

// 把边数组转换成邻接表
vector<vector<int>> edge2Vector(vector<int> sel, vector<tuple<int,int,double>> &Edges, int n)
{
    vector<vector<int>> G(n);
    for(auto idx : sel){
        auto e = Edges[idx];
        int x = get<0>(e), y = get<1>(e);
        G[x].emplace_back(y);
        G[y].emplace_back(x);
    }
    return G;
}

double prob_of_possible_w(vector<int> sel, vector<tuple<int,int,double>> &Edges, int n)
{
    double ans = 1.0;
	unordered_map<int,bool> mp;
	for(auto idx : sel) mp[idx] = true;
	for(int i = 0; i < Edges.size(); i++){
		double prob = get<2>(Edges[i]);
		if(mp[i]) ans *= prob;
		else ans *= 1-prob;
	}
	return ans;
}

/**
  EXACT-POS: The exact algorithm for CSU
  * @params:
  **/

vector<int> EXACT_POS(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      vector<int> &C, int q, int k, double theta,
                      vector<double> &Pr_v)
{
    int n = G.size();
	int m = Edges.size();
	Pr_v.clear();
	unordered_map<int,double> Pr;

	vector<vector<int>> ans; vector<int> temp;
	locals lo;

//    if(C.empty()) return vector<int>{};
//    int remain = C.size();
//    unordered_set<int> set_of_C(C.begin(), C.end());
    vector<int> result;

	for(int i = 1; i <= m; i++){ // [0,m-1]边中随机取i条边
	    cout<<"i="<<i<<"/"<<m<<endl;
		ans.clear(); temp.clear();
		combine(m, i,ans,temp);
		for(auto vs : ans){
			auto H = edge2Vector(vs,Edges,n);
			double prob = prob_of_possible_w(vs,Edges,n);
			auto Cqk = lo.global(q,k,H);
			for(auto u : Cqk) {
			    Pr[u] += prob;
//                if(set_of_C.find(u) != set_of_C.end() &&
//                Pr[u] - prob < theta && Pr[u] >= theta)
                if(Pr[u] - prob < theta && Pr[u] >= theta)
                {
                    result.emplace_back(u);
//                    --remain;
//                    set_of_C.erase(u);
//                    if(remain == 0){
//                        for(auto v : C){
//                            Pr_v.emplace_back(Pr[v]);
//                        }
//                        return result;
//                    }
                }
			}
		}
	}

	for(auto v : C) Pr_v.emplace_back(Pr[v]);

	return result;
}

vector<int> EXACT_POS(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                      int q, int k, double theta,
                      vector<double> &Pr_v)
{
    int n = G.size();
    int m = Edges.size();
    Pr_v.clear(); Pr_v.resize(n);
    unordered_map<int,double> Pr;

    vector<vector<int>> ans; vector<int> temp;
    locals lo;

//    if(C.empty()) return vector<int>{};
//    int remain = C.size();
//    unordered_set<int> set_of_C(C.begin(), C.end());
    vector<int> result;

    for(int i = 1; i <= m; i++){ // [0,m-1]边中随机取i条边
        cout<<"i="<<i<<"/"<<m<<endl;
        ans.clear(); temp.clear();
        combine(m, i,ans,temp);
        for(auto vs : ans){
            auto H = edge2Vector(vs,Edges,n);
            double prob = prob_of_possible_w(vs,Edges,n);
            auto Cqk = lo.global(q,k,H);
            for(auto u : Cqk) {
                Pr[u] += prob;
//                if(set_of_C.find(u) != set_of_C.end() &&
//                Pr[u] - prob < theta && Pr[u] >= theta)
                if(Pr[u] - prob < theta && Pr[u] >= theta)
                {
                    result.emplace_back(u);
//                    --remain;
//                    set_of_C.erase(u);
//                    if(remain == 0){
//                        for(auto v : C){
//                            Pr_v.emplace_back(Pr[v]);
//                        }
//                        return result;
//                    }
                }
            }
        }
    }

//    for(auto v : C) Pr_v.emplace_back(Pr[v]);
    for(auto &[v,prob] : Pr){
        Pr_v[v] = prob;
    }

    return result;
}

/**
 * EXACT-ENUM: 论文中基于深度优先的精确算法
 * ASSERT:未实现，卡在了遍历过的社区子图需要保存
 * @param G
 * @param Edges
 * @param q
 * @param k
 * @param theta
 * @return
 */


void exact_enum_core(vector<tuple<int,int,double>> &Edges, int q, int k)
{
//    double prob = 1.0;
//    unordered_set<int> vs;
//    for(auto e : Edges){
//        prob *= get<2>(e);
//        vs.insert(get<0>(e));
//        vs.insert(get<1>(e));
//    }
//    for(auto v : vs) Pr_v[v] += prob;

    int m = Edges.size();
    locals lo;
    for(int i = 0; i < m; i++){
        vector<tuple<int,int,double>> Edges_t;
        Edges_t.insert(Edges_t.end(), Edges.begin(), Edges.begin()+i);
        Edges_t.insert(Edges_t.end(), Edges.begin()+i+1, Edges.end());
        auto G_t = lo.edges2Table(Edges_t);
        auto Cqk = lo.global(q, k, G_t);
        unordered_set<int> Cqk_set(Cqk.begin(), Cqk.end());
        set<tuple<int,int,double> > temp;
        for(auto e : Edges_t){
            int u = get<0>(e);
            int v = get<1>(e);
            double e_prob = get<2>(e);
            if(Cqk_set.find(u) != Cqk_set.end() && Cqk_set.find(v) != Cqk_set.end()){
                temp.insert(tuple<int,int,double>{u,v,e_prob});
            }
        }
        if(!temp.empty() && find(visited.begin(), visited.end(), temp) == visited.end()){
            ++cnt;
            visited.emplace_back(temp);
            cout<<"cnt= "<<cnt<<",size= "<<visited.size()<<endl;
//            auto G_t_t = lo.extractSubgraph(Cqk, G_t);
//            auto Edges_t_t = lo.table2Edges(G_t_t);
            Edges_t.clear();
            Edges_t.assign(temp.begin(), temp.end());
            exact_enum_core(Edges_t, q, k);
        }
    }
}

/**
 * 用容斥原理(inclusion-exclusion)计算每个顶点的概率
 *
 */
void ie(vector<double> &Pr_v)
{
    int n = visited.size();
    for(int i = 1; i <= n; i++){
        vector<vector<int>> ans;
        vector<int> temp;
        combine(n,i,ans,temp);
        for(auto vs : ans){
            // vs.size == i

            set<tuple<int,int,double> > ss;
            double prob = 1.0;
            unordered_set<int> vert;

            for(auto idx : vs){
                for(auto e : visited[idx]){
                    if(ss.find(e) == ss.end()){
                        prob *= get<2>(e);
//                        vert.insert(get<0>(e));
//                        vert.insert(get<1>(e));
                        ss.insert(e);
                    }
                }
            }

            map<tuple<int,int,double>, int> mp;
            for(auto idx : vs){
                auto subg = visited[idx];
                for(auto m : subg){
                    mp[m]++;
                }
            }

            for(auto &[e,cnt] : mp){
                if(cnt == i){
//                    prob *= get<2>(e);
                    vert.insert(get<0>(e));
                    vert.insert(get<1>(e));
                }
            }

            for(auto v : vert){
                Pr_v[v] += prob * pow(-1,i-1);
            }
        }
    }
}

vector<int> EXACT_ENUM(vector<vector<pairs>> &G, vector<tuple<int,int,double>> &Edges,
                       int q, int k, double theta,
                       vector<double> &Pr_v)
{
    cout<<"EXACT_ENUM"<<endl;

    int n = G.size();
    cnt = 0;
    visited.clear();
    visited.emplace_back(set<tuple<int,int,double>>(Edges.begin(), Edges.end()));
    exact_enum_core(Edges, q, k);
    cout<<"cnt="<<cnt<<endl;
    cout<<"size="<<visited.size()<<endl;

    Pr_v.clear(); Pr_v.resize(n);
    ie(Pr_v);

    vector<int> ret;
    for(int v = 0; v < n; v++){
//        cout<<"v="<<v<<",p="<<Pr_v[v]<<endl;
        if(Pr_v[v] >= theta) ret.emplace_back(v);
    }
    return ret;
}

int main()
{
	string filename = "../data/test0718_2.txt";
	base b;
	auto G = b.readFilep(filename);
	auto Edges = b.readFileE(filename);
	int n = b.V;
	int m = b.E;

	cout<<"V="<<b.V<<",E="<<b.E<<endl;

	int q = 1;
	int k = 2;
	double theta = 0.0;

	locals lo;
	auto Cqk = lo.global(q, k, G);
	G = lo.extractSubgraph(Cqk, G);
	Edges = lo.table2Edges(G);

//	for(auto v : Cqk){
//	    cout<<v<<" ";
//	}
    cout<<"Node size="<<Cqk.size()<<endl;
	cout<<endl;
	cout<<"Edges size="<<Edges.size()<<endl;

	vector<double> Pr_v;
    vector<int> vs;

    vs = EXACT_POS(G, Edges, Cqk, q, k, theta, Pr_v);
	for(int i = 0; i < Pr_v.size(); i++){
	    cout<<"i="<<i<<",p="<<Pr_v[i]<<endl;
	}

	printf("-------------\n");


//	vs = EXACT_ENUM(G, Edges, q, k, theta, Pr_v);
//    for(int i = 0; i < Pr_v.size(); i++){
//        cout<<"i="<<i<<",p="<<Pr_v[i]<<endl;
//    }
//
////    cnt = 1; visited.clear();
////    exact_enum_core(Edges, q, k);
//    cout<<"cnt="<<cnt<<endl;
//    cout<<"size="<<visited.size()<<endl;
//	for(int i = 0; i < Cqk.size(); i++){
//	    cout<<"v="<<Cqk[i]<<",p="<<Pr_v[i]<<endl;
//	}

//    for(auto e_set : visited){
//        for(auto e : e_set){
//            cout<<get<0>(e)<<" "<<get<1>(e)<<", ";
//        }
//        cout<<endl;
//    }
}
