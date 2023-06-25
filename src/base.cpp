#include "../include/base.h"

vector<vector<int>>
base::readFile(string filename)
{
    printf("Reading file ...\n");
    ifstream in(filename);
    string temp;
    int x, y;
    vector<vector<int>> G; this->E = 0;
    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x; ss>>y;
        if(x == y) continue;
        int maxn = max(x, y);
        if((int)G.size()-1 < maxn) G.resize(maxn+1); // 注意size是无符号数
        G[x].emplace_back(y);
        G[y].emplace_back(x);
        this->E++;
    }
    in.close();
    this->V = G.size();

//    // TIPS: 参考<<local search of communities in large graphs>>中的预处理
//    // 按照度降序排序每个顶点的邻接表
////    printf("Sort vertices ...\n");
//    int n = G.size();
//    vector<int> deg(n, 0);
//    for(int i = 0; i < n; i++) deg[i] = G[i].size();
//    for(int i = 0; i < n; i++){
//        printf("Sorting vertex %d/%d\n", i, n-1);
//        sort(G[i].begin(), G[i].end(), [deg](int x, int y){
//            return deg[x] > deg[y];
//        });
//    }

    return G;
}

vector<vector<pairs>> 
base::readFilep(string filename)
{
    ifstream in(filename);
    string temp;
    int x, y; double prob;
    vector<vector<pairs>> G; this->E = 0;
    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x; ss>>y; ss>>prob;
        int maxn = max(x, y);
        if((int)G.size()-1 < maxn) G.resize(maxn+1); // 注意size是无符号数
        G[x].emplace_back(pairs{y, prob});
        G[y].emplace_back(pairs{x, prob});
        this->E++;
    }
    in.close();
    this->V = G.size();

//    // TIPS: 参考<<local search of communities in large graphs>>中的预处理
//    // 按照度降序排序每个顶点的邻接表
////    printf("Sort vertices ...\n");
//    int n = G.size();
//    vector<int> deg(n, 0);
//    for(int i = 0; i < n; i++) deg[i] = G[i].size();
//    for(int i = 0; i < n; i++){
//        printf("Sorting vertex %d/%d\n", i, n-1);
//        sort(G[i].begin(), G[i].end(), [deg](auto &x, auto &y){
//            return deg[x.y] > deg[y.y];
//        });
//    }
//
//    printf("Writing to file ...\n");
//    string outfile = filename + "_sorted.txt";
//    ofstream out(outfile);
//    for(int i = 0; i < n; i++){
//        out<<i<<" ";
//        for(auto ps : G[i]){
//            out<<ps.y<<" "<<ps.prob<<" ";
//        }
//        out<<endl;
//    }

    return G;
}

vector<tuple<int,int,double>>
base::readFileE(string filename)
{
    ifstream in(filename);
    string temp;
    int x, y; double prob;
    vector<tuple<int,int,double>> G; 
    set<int> S;
    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x; ss>>y; ss>>prob;
        G.emplace_back(x, y, prob);
        // S.insert(x); S.insert(y);
    }
    // this->E = G.size();
    // this->V = S.size();
    return G;
}

vector<pair<int,int>>
base::readFileEd(string filename) {
    ifstream in(filename);
    string temp;
    int x, y;
    vector<pair<int,int>> G;
    set<int> S;
    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x; ss>>y;
        G.emplace_back(x, y);
        // S.insert(x); S.insert(y);
    }
    // this->E = G.size();
    // this->V = S.size();
    return G;
}

void
base::writeToFile(string filename, vector<vector<int>> &G)
{
    ofstream out(filename);
    for(int i = 0; i < V; i++){
        out<<i;
        for(auto w : G[i]){
            out<<" -> "<<w;
        }
        out<<" : "<<G[i].size()<<endl;
    }
    out.close();
}

void
base::writeToFile(string filename, vector<int> &vs)
{
    ofstream out(filename);
    for(int i = 0; i < V; i++){
        out<<i<<" "<<vs[i]<<endl;
    }
    out.close();
}

void
base::printg(vector<vector<int>> &G)
{
    printf("\n");
    int n = G.size();
    for(int i = 0; i < n; i++){
        printf("%d", i);
        for(auto c : G[i]){
            printf(" -> %d", c);
        }
        printf("\n");
    }
    printf("\n");
}

void
base::printg(vector<vector<pairs>> &G)
{
    printf("\n");
    int n = G.size();
    for(int i = 0; i < n; i++){
        printf("%d", i);
        for(auto c : G[i]){
            printf(" -> {%.2f,%d}", c.prob, c.y);
        }
        printf(" Deg=%d", (int)G[i].size());
        printf("\n");
    }
    printf("\n");
}

void
base::printv(vector<int> &vs)
{
    printf("\n");
    if(!vs.empty()){
        for(auto c : vs){
            printf("%d ", c);
        }
        printf("\n");
    }
    else{
        printf("Empty set\n");
    }
    printf("\n");
}

int
base::comb(int n, int r) {
    if(n == 0 && r == 0) return 0;
    if(r == 0) return 1;
    return comb(n-1, r-1) + comb(n-1, r);
}

void base::printt(tuple<int, int, double> &t) {
    printf("%d %d %.2f\n", get<0>(t), get<1>(t), get<2>(t));
}

void base::printg(vector<tuple<int, int, double>> &Edges) {
    printf("\n");
    for(auto e : Edges){
        printt(e);
    }
    printf("\n");
}

double base::avgProb(vector<tuple<int,int,double>> &Edges)
{
    double total = 0.0;
    int n = Edges.size();
    for(auto e : Edges){
        double prob = get<2>(e);
        total += prob;
    }
    return total * 1.0 / n;
}

double base::MSE(vector<double> &uc1, vector<double> &uc2) {
    assert(uc1.size() == uc2.size());
    int n = uc1.size();
    double mse = 0.0;
    for(int i = 0; i < n; i++){
        mse += (uc1[i] - uc2[i]) * (uc1[i] - uc2[i]);
    }
    return mse * 1.0 / n;
}

vector<vector<pairs>> base::readFilep_from_exist(string filename) {
    filename = filename + "_sorted.txt";
    ifstream in(filename);
    vector<vector<pairs>> G;
    this->E = 0;
    int x, y;
    double prob;
    string temp;
    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x;
        if((int)G.size()-1 < x) G.resize(x+1);
        while(ss>>y){
            ss>>prob;
            if((int)G.size()-1 < y) G.resize(y+1);
            G[x].emplace_back(pairs{y, prob});
            if(x < y) this->E++;
        }
    }
    this->V = G.size();
    return G;
}

bool base::graphIsomorphism(vector<vector<pairs>> &G1, vector<vector<pairs>> &G2) {
    int n = G1.size();
    if(G2.size() != n) return false;
    for(int i = 0; i < n; i++){
        int m = G1[i].size();
        if(G2[i].size() != m) return false;
        for(int j = 0; j < m; j++){
            auto p1 = G1[i][j], p2 = G2[i][j];
            if(p1.y != p2.y || p1.prob != p2.prob) {
                cout<<"p1: "<<p1.y<<" "<<p1.prob<<endl;
                cout<<"p2: "<<p2.y<<" "<<p2.prob<<endl;
                return false;
            }
        }
    }
    return true;
}

double base::norm2(vector<double> &Pr) {
    double ret = 0.0;
    for(auto p : Pr){
        ret += p * p;
    }
    return sqrt(ret);
}

double base::cosine_dis(vector<double> &Pr1, vector<double> &Pr2) {
    double norm_of_Pr1 = norm2(Pr1), norm_of_Pr2 = norm2(Pr2);
    assert(norm_of_Pr1 > 0 && norm_of_Pr2 > 0);
    double ret = 0.0;
    int n = Pr1.size();
    assert(Pr2.size() == n);
    for(int i = 0; i < n; i++) ret += Pr1[i] * Pr2[i];
    return ret / 1.0 / norm_of_Pr1 / norm_of_Pr2;
}

/**
 * Referenced from <<On efficiently finding reverse k-nearest neighbors
 * over uncertain graphs>>, VLDBJ, 2017
 * @param Prs: 概率向量的集合
 * @return 估计方差
 */

double base::var(vector<vector<double>> &Prs) // TIPS:从引用传递改为赋值传递
{
    int n0 = Prs.size();
    vector<double> Pr_avg;
    for(auto &Pr : Prs){
        if(Pr_avg.empty()) Pr_avg = Pr;
        else{
            for(int i = 0; i < Pr.size(); i++) Pr_avg[i] += Pr[i];
        }
    }

    for(auto &p : Pr_avg) p /= n0 * 1.0;

    printf("pr_avg: ");
    for(auto p : Pr_avg) cout<<p<<" ";
    cout<<endl;

    vector<double> Cs;

    for(auto &Pr : Prs){
        Cs.emplace_back(cosine_dis(Pr, Pr_avg));
    }

    double C_avg = accumulate(Cs.begin(), Cs.end(), 0.0);
    C_avg /= 1.0 * n0;

    cout<<"c_avg: "<<C_avg<<endl;

    double ret = 0.0;
    for(auto Ci : Cs){
        ret += (Ci - C_avg) * (Ci - C_avg);
    }

    printf("cs: ");
    for(auto ci : Cs){
        cout<<ci<<" ";
    }
    cout<<endl;

    return ret / 1.0 / (n0-1);
}

vector<vector<int>> base::toDeter(vector<vector<pairs>> &G) {
    int n = G.size();
    vector<vector<int>> ret(n);
    for(int i = 0; i < n; i++){
        auto vs = G[i];
        for(auto ps : vs){
            ret[i].emplace_back(ps.y);
        }
    }
    return ret;
}