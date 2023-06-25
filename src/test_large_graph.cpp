#include <bits/stdc++.h>

using namespace std;

struct Triple
{
    int x, y;
    double prob;
};

struct Edge
{
    int x, y;
};

class RelPrune
{
public:
    vector<Triple> G;
    int n;
    int m;
    double maxProb, minProb;
    // unordered_map<int,bool> candmp; // 社区候选集哈希表
    unordered_map<int,int> degmp; // 社区候选集内部度

    void buildGraph(vector<Triple> &input);
    vector<Edge> sampleG();

    vector<int> findConnectedComponent(int q, vector<Edge> &iG);
    vector<int> generateCandidate(int q, double p0, int s);
    vector<Triple> readFile(string filename);
    void graphInfo();

    vector<vector<int>> sampleH(vector<int> &cand);
    vector<int> findCommunity(int q, int k, double theta, int s, vector<int> &cand);
    bool check(int k, vector<int> &cand2);
    void insert_update(int w, vector<vector<int>> &iG);
    void delete_update(int w, vector<vector<int>> &iG);
    int degree(int q);
private:
    int findRoot(int x, vector<int> &f);
    void merge(int x, int y, vector<int> &f);
};

void RelPrune::buildGraph(vector<Triple> &input)
{
    G = input;
}

vector<Edge> RelPrune::sampleG()
{
    vector<Edge> ans;
    double prob;
    for(int i = 0; i < G.size(); i++){
        prob = rand() / (double)RAND_MAX;
        if(G[i].prob > prob) ans.emplace_back(Edge{G[i].x, G[i].y});
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

vector<int> RelPrune::findConnectedComponent(int q, vector<Edge> &iG)
{
    vector<int> f(n);
    for(int i = 0; i < n; i++) f[i] = i;
    
    for(auto vs : iG){
        merge(vs.x, vs.y, f);
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
    if(minn == 0){
        for(int i = 0; i < n; i++) ans.emplace_back(i);
        return ans;
    }

    clock_t start, end;
    for(int i = 0; i < s; i++){
        printf("%d/%d iteration. \n", i+1, s);
        vector<Edge> iG = sampleG();
        vector<int> cc = findConnectedComponent(q, iG);
        for(auto c : cc){
            nums[c]++;
            if(nums[c] == minn) ans.emplace_back(c);
        }
    }
    // sort(ans.begin(), ans.end(), less<int>());
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

    int maxn = -1;
    int cnt = 0;
    double maxProb = -1, minProb = 2;

    while(getline(in, temp)){
        stringstream ss(temp);
        ss>>x; ss>>y;
        ss>>prob;
        ans.emplace_back(Triple{x, y, prob});
        cnt++;
        if(x > maxn) maxn = x;
        if(y > maxn) maxn = y;
        if(prob > maxProb) maxProb = prob;
        if(prob < minProb) minProb = prob;
    }

    this->n = maxn + 1;
    this->m = cnt;
    this->maxProb = maxProb;
    this->minProb = minProb;

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

//
vector<vector<int>> RelPrune::sampleH(vector<int> &cand)
{
    vector<bool> sig(n, false);
    for(auto c : cand) sig[c] = true;

    vector<vector<int>> ans(n);
    double prob;
    int x, y;
    for(int i = 0; i < G.size(); i++)
    {
        prob = rand() / (double)RAND_MAX;
        if(sig[G[i].x] && sig[G[i].y] && G[i].prob > prob){
            x = G[i].x; y = G[i].y;
            ans[x].emplace_back(y);
            ans[y].emplace_back(x);
        }
    }
    return ans;
}

bool RelPrune::check(int k, vector<int> &cand2)
{
    int minn = n + 1;
    for(auto c : cand2){
        if(degmp[c] < minn) minn = degmp[c];
    }
    return minn >= k;
}

void RelPrune::insert_update(int w, vector<vector<int>> &iG)
{
    auto vs = iG[w];
    degmp.insert(make_pair(w, 0));
    // candmp.insert(make_pair(w, true));
    for(auto c : vs){
        if(degmp.count(c)){
            degmp[c]++;
            degmp[w]++;
        }
    }
}

void RelPrune::delete_update(int w, vector<vector<int>> &iG)
{
    auto vs = iG[w];
    for(auto c : vs){
        if(degmp.count(c)){
            degmp[c]--;
        }
    }
    degmp.erase(w);
}

vector<int> RelPrune::findCommunity(int q, int k, double theta, int s, vector<int> &cand)
{
    unordered_map<int,int> nums;

    for(int i = 0; i < s; i++){
        printf("%d/%d iteration.\n", i+1, s);
        auto iG = sampleH(cand);
        // local
        degmp.clear();
        vector<int> cand2;
        unordered_map<int,bool> visit;
        queue<int> Q; Q.push(q);
        visit[q] = true;
        bool flag = false;

        int f1, f2, f3;
        f1 = f2 = 0;
        while(!Q.empty()){
            printf("loop1: %d\n", ++f1);
            int temp = Q.front();
            Q.pop();
            cand2.emplace_back(temp);
            insert_update(temp, iG);
            flag = check(k, cand2);
            if(flag){
                for(auto c : cand2){
                    nums[c]++; // 并非最大社区，提前返回了结果
                }
                break;
            }
            auto vs = iG[temp];
            for(auto c : vs){
                if(!visit[c] && iG[c].size() >= k){
                    Q.push(c);
                    visit[c] = true;
                }
            }
        }

        while(!flag && !cand2.empty()){
            printf("loop2: %d\n", ++f2);
            sort(cand2.begin(), cand2.end(), [&](int x, int y){
                return degmp[x] > degmp[y];
            }); 
            vector<int> temp;
            for(int i = cand2.size()-1; i >= 0; i--){
                if(degmp[cand2[i]] < k) temp.emplace_back(cand2[i]);
                else break;
            }
            if(temp.empty()) break;
            for(auto c : temp){
                delete_update(c, iG);
                cand2.pop_back();
            }
            flag = check(k, cand2);
        }

        if(!cand2.empty()){
            for(auto c : cand2){
                nums[c]++;
            }
        }
    }

    vector<int> ans;
    int minn = theta * s;
    for(auto [v, num] : nums){
        if(num >= minn) ans.emplace_back(v);
    }
    assert(ans.empty() || find(ans.begin(), ans.end(), q) != ans.end());
    return ans;
}

int RelPrune::degree(int q)
{
    int ans = 0;
    for(auto t : G){
        if(t.x == q || t.y == q) ans++;
    }
    return ans;
}

int main(void)
{
    RelPrune relp;
    int x, y;
    double prob;
    
    string filename = "biomine.txt";
    vector<Triple> input = relp.readFile(filename);

    // for(auto t : input){
    //     printf("%d %d %.2f\n", t.x, t.y, t.prob);
    // }

    relp.buildGraph(input);
    // relp.showG();
    relp.graphInfo();

    int q;
    double p0, theta; 
    int s, k;

    clock_t start, end;

    while(1){
        printf("Input q, p0 and s: \n");
        scanf("%d", &q);
        if(q == -1) break;
        printf("degree = %d\n", relp.degree(q));
        scanf("%lf %d", &p0, &s);

        start = clock();
        vector<int> cand = relp.generateCandidate(q, p0, s);
        end = clock();

        printf("Candidate size: %d\n", (int)cand.size());
        printf("Number of pruned vertices: %d\n", (int)(relp.n - cand.size()));

        printf("Total time: %.0f ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);

        printf("Input k, theta and s: \n");
        scanf("%d %lf %d", &k, &theta, &s);

        start = clock();
        vector<int> community = relp.findCommunity(q, k, theta, s, cand);
        end = clock();

        printf("Community: ");
        for(auto c : community) printf("%d ", c);
        printf("\n");
        printf("Community shape: %d\n", (int)community.size());
        printf("Number of pruned vertices: %d\n", (int)(cand.size() - community.size()));

        printf("Total time: %.0f ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);


    }


    // for(auto c : cand){
    //     printf("%d ", c);
    // }
    // printf("\n");
}