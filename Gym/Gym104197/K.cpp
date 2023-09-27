#include <bits/stdc++.h>

using i64 = std::int64_t;

template <typename G>
struct StronglyConnectedComponents {
   private:
    const G &g;
    std::vector<std::vector<int>> rg;
    std::vector<int> comp, order;
    std::vector<char> used;
    std::vector<std::vector<int>> blng;

   public:
    std::vector<std::vector<int>> dag;
    StronglyConnectedComponents(G &_g) : g(_g), used(g.size(), 0) { build(); }

    int operator[](int k) { return comp[k]; }

    std::vector<int> &belong(int i) { return blng[i]; }

   private:
    void dfs(int idx) {
        if (used[idx]) return;
        used[idx] = true;
        for (auto to : g[idx]) dfs(int(to));
        order.push_back(idx);
    }

    void rdfs(int idx, int cnt) {
        if (comp[idx] != -1) return;
        comp[idx] = cnt;
        for (int to : rg[idx]) rdfs(to, cnt);
    }

    void build() {
        for (int i = 0; i < (int)g.size(); i++) dfs(i);
        std::reverse(begin(order), end(order));
        used.clear();
        used.shrink_to_fit();

        comp.resize(g.size(), -1);

        rg.resize(g.size());
        for (int i = 0; i < (int)g.size(); i++) {
            for (auto e : g[i]) {
                rg[(int)e].emplace_back(i);
            }
        }
        int ptr = 0;
        for (int i : order)
            if (comp[i] == -1) rdfs(i, ptr), ptr++;
        rg.clear();
        rg.shrink_to_fit();
        order.clear();
        order.shrink_to_fit();

        dag.resize(ptr);
        blng.resize(ptr);
        for (int i = 0; i < (int)g.size(); i++) {
            blng[comp[i]].push_back(i);
            for (auto &to : g[i]) {
                int x = comp[i], y = comp[to];
                if (x == y) continue;
                dag[x].push_back(y);
            }
        }
    }
};
void solve() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        a--, b--;
        adj[a].push_back(b);
    }

    StronglyConnectedComponents SCC(adj);
    if (SCC.dag.size() == 1)
        std::cout << "YES\n";
    else
        std::cout << "NO\n";

    return;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    std::cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}