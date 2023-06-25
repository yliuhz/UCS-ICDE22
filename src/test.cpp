#include <bits/stdc++.h>

using namespace std;

struct Triple
{
    int x, y;
    double prob;
};

class RelPrune
{
public:
    vector<vector<double>> G;
    int n;
    int m;
    double maxProb, minProb;
    void buildGraph(int n, vector<Triple> &input);
    vector<vector<int>> sampleG();
    void showG();
    void showG(vector<vector<double>> &dG);
    void showG(vector<vector<int>> &iG);
    vector<int> findConnectedComponent(int q, vector<vector<int>> &iG);
    vector<int> generateCandidate(int q, double p0, int s);
    vector<Triple> readFile(string filename);
    void graphInfo();
private:
    int findRoot(int x, vector<int> &f);
    void merge(int x, int y, vector<int> &f);
};

void RelPrune::buildGraph(int n, vector<Triple> &input)
{
    G.resize(n);
    for(int i = 0; i < n; i++) G[i].resize(n);
    this->n = n;
    this->m = input.size();
    this->maxProb = -1; this->minProb = 2;
    for(auto t : input)
    {
        int x = t.x, y = t.y;
        double prob = t.prob;
        G[x][y] = G[y][x] = prob;
        if(prob > maxProb) maxProb = prob;
        if(prob < minProb) minProb = prob;
    }
}

vector<vector<int>> RelPrune::sampleG()
{
    vector<vector<int>> ans(n, vector<int>(n, 0));
    double prob = rand() / (double)RAND_MAX;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            if(G[i][j] > prob) ans[i][j] = 1;
        }
    }
    return ans;
}

int RelPrune::findRoot(int x, vector<int> &f)
{
    if(f[x] != x){
        int root = findRoot(f[x], f);
        f[x] = root;
    }
    return f[x];
}

void RelPrune::merge(int x, int y, vector<int> &f)
{
    int r1 = findRoot(x, f);
    int r2 = findRoot(y, f);

    if(r1 != r2){
        f[r1] = r2;
    }
}

vector<int> RelPrune::findConnectedComponent(int q, vector<vector<int>> &iG)
{
    vector<int> f(n);
    for(int i = 0; i < n; i++) f[i] = i;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            if(iG[i][j]) merge(i, j, f);
        }
    }
    vector<int> ans;
    int r0 = findRoot(q, f);
    for(int i = 0; i < n; i++){
        int r1 = findRoot(i, f);
        if(r1 == r0) ans.emplace_back(i);
    }
    return ans;
}

vector<int> RelPrune::generateCandidate(int q, double p0, int s)
{
    int minn = p0 * s;
    vector<int> nums(n, 0);
    vector<int> ans;
    for(int i = 0; i < s; i++){
        vector<vector<int>> iG = sampleG();
        vector<int> cc = findConnectedComponent(q, iG);
        for(auto c : cc){
            nums[c]++;
            if(nums[c] == minn) ans.emplace_back(c);
        }
    }
    sort(ans.begin(), ans.end(), less<int>());
    return ans;
}

vector<Triple> RelPrune::readFile(string filename)
{
    ifstream in(filename);
    if(!in.is_open()){
        printf("Open file %s Failed.\n", filename.c_str());
    }

    vector<Triple> ans;
    int x, y;
    double prob;
    string temp;
    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x; ss>>y;
        ss>>prob;
        ans.emplace_back(Triple{x, y, prob});
    }

    in.close();
    return ans;
}

void RelPrune::graphInfo()
{
    printf("Number of vertices: %d\n", n);
    printf("Number of edges: %d\n", m);
    printf("Largest probability: %.2f\n", maxProb);
    printf("Least probability: %.2f\n", minProb);
}

void RelPrune::showG(vector<vector<double>> &dG)
{
    printf("-------------\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%3.2f ", dG[i][j]);
        }
        printf("\n");
    }
    printf("--------------\n");
}

void RelPrune::showG(vector<vector<int>> &iG)
{
    printf("-------------\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%1d ", iG[i][j]);
        }
        printf("\n");
    }
    printf("--------------\n");
}

void RelPrune::showG()
{
    showG(G);
}

int main(void)
{
    RelPrune relp;
    int n = 1737984;
    int x, y;
    double prob;
    
    string filename = "biomine.txt";
    vector<Triple> input = relp.readFile(filename);

    // for(auto t : input){
    //     printf("%d %d %.2f\n", t.x, t.y, t.prob);
    // }

    relp.buildGraph(n, input);
    // relp.showG();
    relp.graphInfo();

    // vector<int> cand = relp.generateCandidate(0, 0.7, 1000);

    // for(auto c : cand){
    //     printf("%d ", c);
    // }
    // printf("\n");
}